#include <stdint.h>
#include <stddef.h>

#include "ata.h"
#include "pata.h"

#include "drivers/io/io.h"


uint16_t id_buff[256];


void ATA_flip_string(char* str, size_t len){
    for(size_t i = 1; i<len; i+=2){
        char tmp = str[i];
        str[i] = str[i-1];
        str[i-1] = tmp;
    }
}


void ATA_write_sectors(uint8_t drive, uint32_t start_sector, uint8_t count, void* buffer, pata_callback_t callback){
    if(drive >= 0 && drive <=3){
        PATA_write_pio(drive, start_sector, count, buffer, callback);
    }
}

void ATA_read_sectors(uint8_t drive, uint32_t start_sector, uint8_t count, void* buffer, pata_callback_t callback){
    if(drive >= 0 && drive <=3){
        PATA_read_pio(drive, start_sector, count, buffer, callback);
    }
}




ata_device_type_t ATA_probe_sync(uint8_t drive, uint16_t* buff){
    if(drive >= 0 && drive <=3){
        for(short int i = 0; i < 2; ++i){
            ata_error_t code = PATA_identify_sync(drive, id_buff);
            
            
            if (code == 0){
                return ATA_DEV_TYPE_PATA;
            }


            if(code & ATA_ERR_ABRT){
                return ATA_DEV_TYPE_ERR;
            }

            if (code & ATA_ERR_FLOAT){
                return ATA_DEV_TYPE_NONE;
            }

            // If timeout or generic error, reset drive and try again. 
            // If error happens during reset, set error disk and exit loop.
            if ((code & ATA_ERR_GEN) || (code & ATA_ERR_TIME)) {
                if(PATA_reset_soft(drive) == 0) continue;
                return ATA_DEV_TYPE_ERR;
            }
        }


        // In case of two consecutive timeouts on the same drive
        return ATA_DEV_TYPE_ERR;
    }
}