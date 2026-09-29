#include "pata.h"
#include "pata_generic.h"
#include "ata_defs.h"

#include <stdint.h>

#include "core/timer/timer.h"
#include "drivers/io/io.h"



/**
 * * @brief Synchronously sends the identify command.
 * 
 * * @param drive  The drive index (0: Primary Master, 1: Primary Slave, 2: Secondary Master, 3: Secondary Slave).
 * * @param buffer A pointer to a 512-byte buffer to store the IDENTIFY data.
 * * @return ata_err_t (lower byte: standard ata error, bit8: ata error reported, bit9: floating buffer detected, bit10: timeout)
 */
ata_error_t PATA_identify_sync(uint8_t drive, void* buffer){
    /* *
     * PATA supports two channels (Primary/Secondary), each with a Master and Slave.
     * In the adopted convention, drive 0-1 map to Primary; 2-3 map to Secondary.
     */
    enum pata_port port = (drive < 2) ? PATA_PRIMARY : PATA_SECONDARY;
    pata_operation_t* op = (drive < 2) ? &pata_primary_op : &pata_secondary_op;


    if (drive >= 2) drive -= 2; // Normalize drive index to 0 (Master) or 1 (Slave) for the selected port
    

    /* * Drive Selection
     * Register bits for Drive/Head register:
     * Bit 4: 0 for Master, 1 for Slave.
     * Bits 5 & 7: Hardcoded to 1 for legacy compatibility.
     * Bit 6: LBA mode (0 for IDENTIFY as per ATA spec).
     */
    uint8_t drive_select = ((drive << 4) & 0b00010000) | 0b10100000;
    outb(port + PATA_OFST_DRIVE_REG, drive_select);


    /* * 
     * If the status register returns 0xFF, the bus is floating, 
     * meaning no drive is physically connected to this port.
     */
    if(PATA_get_status(port, 1) == 0xFF) return ATA_ERR_FLOAT;

    
    /* *
     * For IDENTIFY, the Sector Count and LBA registers must be zeroed.
     */
    outb(port + PATA_OFST_LBAlo_REG, 0);
    outb(port + PATA_OFST_LBAmid_REG, 0);
    outb(port + PATA_OFST_LBAhi_REG, 0);

    outb(port + PATA_OFST_COMMAND_REG, ATA_COMMAND_IDENTIFY);
    

    /* *
     * Wait for the BSY (Busy) bit to clear. 
     * Timeout is set to 31 seconds (conservative estimate for spin-up).
     */
    uint64_t timeout = system_ticks + 31000;
    while(1){
        if(system_ticks >= timeout) return ATA_ERR_TIME;

        uint8_t status = inb(port + PATA_OFST_STATUS_REG);
        
        if(status & ATA_STATUS_DF) return ATA_ERR_GEN;  // Drive Fault (DF)

        if(status & ATA_STATUS_ERR) return ATA_ERR_GEN | inb(port + PATA_OFST_ERROR_REG);

        // Handle generic Error (ERR). Return ERR_GEN combined with Error Register value.
        if(status & ATA_STATUS_ERR) return ATA_ERR_GEN | inb(port + PATA_OFST_ERROR_REG);

        if(!(status & ATA_STATUS_BSY)) break;

        asm volatile("pause");
    }

    /**
     * Wait for data request
     */
    while (!(inb(port + PATA_OFST_STATUS_REG) & ATA_STATUS_DRQ)) {
        if(system_ticks >= timeout) return ATA_ERR_TIME;
        asm volatile("pause");
    }

    PATA_read_sector(buffer, port);
}





