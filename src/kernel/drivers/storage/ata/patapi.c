#include "patapi.h"
#include "ata.h"
#include "core/timer/timer.h"



/**
 * * @brief Synchronously sends the identify command.
 * 
 * * @param drive  The drive index (0: Primary Master, 1: Primary Slave, 2: Secondary Master, 3: Secondary Slave).
 * * @param buffer A pointer to a 512-byte buffer to store the IDENTIFY data.
 * * @return ata_err_t (lower byte: standard ata error, bit8: ata error reported, bit9: floating buffer detected, bit10: timeout)
 */
ata_error_t PATAPI_identify_sync(uint8_t drive, void* buffer){
    
}

