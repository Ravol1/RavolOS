#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stddef.h>
#include "pata.h"
#include "pata_generic.h"
#include "ata.h"
#include "drivers/io/io.h"
#include "core/timer/timer.h"
#include "drivers/video/video.h"



pata_operation_t pata_primary_queue[16];
pata_operation_t pata_secondary_queue[16];

pata_operation_t pata_primary_op = {0};
pata_operation_t pata_secondary_op = {0};


bool PATA_status_df(uint8_t status){
    return (status & 0b00100000);
}

bool PATA_status_err(uint8_t status){
    return (status & 0b0001);
}

bool PATA_status_bsy(uint8_t status){
    return status & 0b10000000; // bit 7
}

bool PATA_status_rdy(uint8_t status){
    return status & 0b01000000; // bit 6
}

bool PATA_status_drq(uint8_t status){
    return status & 0b1000;
}


uint8_t PATA_get_status(enum pata_port port, bool alt){
    enum pata_port_ofst ofst = PATA_OFST_STATUS_REG;
    if(alt) ofst = PATA_OFST_ALT_STATUS_REG;    
    
    return inb(port + ofst);
}


int PATA_read_sector(void* buffer, enum pata_port port){
    uint16_t* buff = (uint16_t*)buffer;

    for(uint16_t i = 0; i<256; i++){
        buff[i] = inw(port + PATA_OFST_DATA_REG);
    }
}

int PATA_write_sector(void* buffer, enum pata_port port){
    uint16_t* buff = (uint16_t*)buffer;

    for(uint16_t i = 0; i<256; i++){
        outw(port + PATA_OFST_DATA_REG, buff[i]);
    }

    return 0;
}


void PATA_read_pio(uint8_t drive, uint32_t start_sector, uint8_t count, void* buffer, pata_callback_t callback) {
    // Determine if we are using the Primary or Secondary IO port base
    enum pata_port port = (drive < 2) ? PATA_PRIMARY : PATA_SECONDARY;
    pata_operation_t* op = (drive < 2) ? &pata_primary_op : &pata_secondary_op;
    if (drive >= 2) drive -= 2; // Normalize drive index to 0 (Master) or 1 (Slave) for the selected port


    op->buffer=buffer;
    op->operation_type = PATA_OP_READ;
    op->total_sectors = count;
    op->current_sector = 0;
    op->operation_status = PATA_OPSTATUS_PENDING;
    op->error_code = 0;
    op->on_done = callback;


    // INPUT STRUCTURE BY BYTE
    // 00h  : Not used / N/A
    // 01h  : Sector count (number of sectors to read)
    // 02h  : LBA low  byte (bits 0–7 of LBA)
    // 03h  : LBA mid  byte (bits 8–15 of LBA)
    // 04h  : LBA high byte (bits 16–23 of LBA)
    // 05h  : Device/Head register:
    //        bit 7     =   1 to enable LBA mode
    //        bits 6–4  =   LBA bits 24–26 (highest 3 bits of LBA28)
    //        bits 3–1  =   reserved / must be 0
    //        bit 0     =   device select (0 = Master, 1 = Slave)
    int8_t device_byte = 0b11100000 | ((drive << 4) & 0b1000) | ((start_sector >> 24) & 0b0111);

    outb(port + PATA_OFST_DRIVE_REG, device_byte);
    io_wait();
    outb(port + PATA_OFST_SECTOR_COUNT_REG, count);                                    
    outb(port + PATA_OFST_LBAlo_REG, (uint8_t)(start_sector));                  
    outb(port + PATA_OFST_LBAmid_REG, (uint8_t)(start_sector >> 8));         
    outb(port + PATA_OFST_LBAhi_REG, (uint8_t)(start_sector >> 16));      
    outb(port + PATA_OFST_COMMAND_REG, ATA_COMMAND_READ_SECTORS);
}


void  PATA_write_pio(uint8_t drive, uint32_t start_sector, uint8_t count, void* buffer, pata_callback_t callback){
    // Determine if we are using the Primary or Secondary IO port base
    enum pata_port port = (drive < 2) ? PATA_PRIMARY : PATA_SECONDARY;
    pata_operation_t* op = (drive < 2) ? &pata_primary_op : &pata_secondary_op;
    if (drive >= 2) drive -= 2; // Normalize drive index to 0 (Master) or 1 (Slave) for the selected port


    op->buffer=buffer;
    op->operation_type = PATA_OP_WRITE;
    op->total_sectors = count;
    op->current_sector = 1;
    op->operation_status = PATA_OPSTATUS_PENDING;
    op->error_code = 0;
    op->on_done = callback;


    // INPUT STRUCTURE BY BYTE
    // 00h  : Not used / N/A
    // 01h  : Sector count (number of sectors to read)
    // 02h  : LBA low  byte (bits 0–7 of LBA)
    // 03h  : LBA mid  byte (bits 8–15 of LBA)
    // 04h  : LBA high byte (bits 16–23 of LBA)
    // 05h  : Device/Head register:
    //        bit 7     =   1 to enable LBA mode
    //        bits 6–4  =   LBA bits 24–26 (highest 3 bits of LBA28)
    //        bits 3–1  =   reserved / must be 0
    //        bit 0     =   device select (0 = Master, 1 = Slave)
    int8_t device_byte = 0b11100000 | ((drive << 4) & 0b1000) | ((start_sector >> 24) & 0b0111);


    outb(port + PATA_OFST_DRIVE_REG, device_byte);
    io_wait();
    outb(port + PATA_OFST_SECTOR_COUNT_REG, count);                                    
    outb(port + PATA_OFST_LBAlo_REG, (uint8_t)(start_sector));                  
    outb(port + PATA_OFST_LBAmid_REG, (uint8_t)(start_sector >> 8));         
    outb(port + PATA_OFST_LBAhi_REG, (uint8_t)(start_sector >> 16));      
    outb(port + PATA_OFST_COMMAND_REG, ATA_COMMAND_WRITE_SECTORS);
    io_wait();
    
    while(1){
        uint8_t status = inb(port + PATA_OFST_ALT_STATUS_REG);
        if(!PATA_status_bsy(status) && PATA_status_drq(status)) break;
        asm volatile("pause");
    }

    uint16_t* buff = (uint16_t*)buffer;

    PATA_write_sector(buff, port);
}


ata_error_t PATA_reset_soft(uint8_t drive){
    enum pata_port port = (drive < 2) ? PATA_PRIMARY : PATA_SECONDARY;
    pata_operation_t* op = (drive < 2) ? &pata_primary_op : &pata_secondary_op;
    if (drive >= 2) drive -= 2; // Normalize drive index to 0 (Master) or 1 (Slave) for the selected port


    uint8_t control = inb(port + PATA_OFST_DEVICE_CONTROL_REG);
    outb(port + PATA_OFST_DEVICE_CONTROL_REG, control | 0x04);
    for(uint8_t i = 0; i<10; ++i){
        io_wait();
    }

    control = inb(port + PATA_OFST_DEVICE_CONTROL_REG);
    outb(port + PATA_OFST_DEVICE_CONTROL_REG, control & 0x11111011);

}



ata_error_t PATA_reset_hard(uint8_t drive){
    enum pata_port port = (drive < 2) ? PATA_PRIMARY : PATA_SECONDARY;
    pata_operation_t* op = (drive < 2) ? &pata_primary_op : &pata_secondary_op;
    if (drive >= 2) drive -= 2; // Normalize drive index to 0 (Master) or 1 (Slave) for the selected port


    // Select the drive. 
    // Bit 4: Drive select (0=Master, 1=Slave)
    // Bits 5 and 7: Must be 1
    // Bit 6: LBA enable (set to 0)
    uint8_t drive_select = ((drive << 4) & 0b00010000) | 0b10100000;
    outb(port + PATA_OFST_DRIVE_REG, drive_select);


    // Reset LBA registers before sending RESET command
    outb(port + PATA_OFST_LBAlo_REG, 0);
    outb(port + PATA_OFST_LBAmid_REG, 0);
    outb(port + PATA_OFST_LBAhi_REG, 0);


    outb(port + PATA_OFST_COMMAND_REG, ATA_COMMAND_DEVICE_RESET);

    uint64_t timeout = system_ticks + 31000; // 31 seconds, as for ATA standards
    while (1){
        if(system_ticks >= timeout) return ATA_ERR_TIME;
        uint8_t status = inb(port + PATA_OFST_STATUS_REG);
        if(!PATA_status_bsy(status)) break;
        if(!PATA_status_err) return inb(port + PATA_OFST_ERROR_REG);


        asm volatile("pause");
    }

    return 0;
}





/**
 * @brief Handles the actual data transfer for multi-sector Read/Write operations.
 * 
 * * @param op     Pointer to the active PATA operation structure.
 * @param status Current value of the status register.
 * @param port   The IO port base (Primary or Secondary).
 * @return int   0 on success, negative values on state mismatch.
 */
int PATA_hadle_readwrite(pata_operation_t* op, uint8_t status, enum pata_port port){
    // Check if there are still sectors remaining in this request
    if(op->current_sector < op->total_sectors){

        // Ensure the drive is actually ready to transfer data (DRQ bit)
        if(!PATA_status_drq(status)) return -2;  // Error: Software expects data, but hardware DRQ is low
        

        uint16_t* buffer = op->buffer + (256 * op->current_sector);
        switch (op->operation_type)
        {
        case PATA_OP_WRITE:         
            PATA_write_sector(buffer, port);
            op->current_sector++;
            break;

        case PATA_OP_READ:
            PATA_read_sector(buffer, port);
            op->current_sector++;
            break;


        default:
            break;
        }

        
    } else{

        // All sectors processed; verify the drive has dropped the Data Request (DRQ)
        uint8_t new_status = PATA_get_status(port, 0);

        if(PATA_status_drq(new_status)) return -3; // Error: Extra data remains on drive

        op->operation_status = PATA_OPSTATUS_DONE;
    }

    return 0;
}


/**
 * @brief High-level state machine for processing PATA command transitions.
 * * Return type for debug only, everything should be handled in here. Change return type to void
 * 
 * * @param op     Pointer to the active PATA operation structure.
 * @param status Current value of the status register.
 * @param port   The IO port base (Primary or Secondary).
 * * @return int Status code indicating the result of the handling:
 * 0  : Success. Operation progressed or completed normally.
 * -1 : Hardware Error. ERR bit was set; error_code was updated from register.
 * 1  : State Mismatch. IRQ received for an operation not marked PENDING.
 * 2  : Unimplemented. The operation_type is unknown or not handled.
 */
int PATA_operation_hanlder(pata_operation_t* op, uint8_t status, enum pata_port port){    

    // Check for Drive Fault
    if(PATA_status_df(status)){
        pata_primary_op.operation_status = PATA_OPSTATUS_ERROR;
        pata_primary_op.error_code = 0xFF;
        
        if(op->on_done){
            op->on_done(op);
        }
    }

    // Check for Error
    if(PATA_status_err(status)){
        pata_primary_op.operation_status = PATA_OPSTATUS_ERROR;
        pata_primary_op.error_code = inb(port + PATA_OFST_ERROR_REG);
        
        if(op->on_done){
            op->on_done(op);
        }
        

        // TODO: Error cleaning code

        return -1;
    }

    // If an interrupt fires but the task is already finished, ignore it
    if(op->operation_status != PATA_OPSTATUS_PENDING) return 1;


    switch (op->operation_type)
    {
        case PATA_OP_READ:
        case PATA_OP_WRITE: 
        {
            int ret = PATA_hadle_readwrite(op, status, port);
            if(ret != 0) return ret;
            break;
        }
        case PATA_OP_IDENTIFY_SYNC:
            op->operation_status = PATA_OPSTATUS_DONE;
            break;

        case PATA_OP_IDENTIFY_ASYNC:
            return 2;

        default:
            return 2;
    }

    // Finalize operation: trigger callback if the task just completed
    if(op->operation_status = PATA_OPSTATUS_DONE) {
        if(op->on_done){
            op->on_done(op);
        }

        // TODO: Implement queue based operations
    }

    return 0;
}


/**
 * @brief Top-level Interrupt Service Routine (ISR) for the Primary PATA Bus.
*/
int PATA_primary_irq_handler(){
    uint8_t status = PATA_get_status(PATA_PRIMARY, 0); 

    if(PATA_status_bsy(status)) return -2;

    int out = PATA_operation_hanlder(&pata_primary_op, status, PATA_PRIMARY);


    return 0;
}
