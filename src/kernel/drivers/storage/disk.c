#include "disk.h"

#include "core/memory/memory.h"
#include "drivers/storage/ata/ata.h"
#include "drivers/storage/ata/ata_defs.h"


enum disk_index {
    DISK_IDX_IDE_PRIMARY_MASTER = 0,
    DISK_IDX_IDE_PRIMARY_SLAVE = 1,
    DISK_IDX_IDE_SECONDARY_MASTER = 2,
    DISK_IDX_IDE_SECONDARY_SLAVE = 3,
    DISK_IDX_FLOPPY_A = 4,
    DISK_IDX_FLOPPY_B = 5,
    DISK_IDX_SATA_START = 6,
    DISK_IDX_MAX = 16
};


disk_t disks[DISK_IDX_MAX];
uint16_t read_buffer[256];

void disk_init(){
    // Prepare the structure
    memset(disks, 0, sizeof(disks));


    uint8_t tries = 0;
    for(int i = 0; i<4; ++i){
        disk_type_t type = ATA_probe_sync(i, read_buffer);

        // If error, probe again. Max two tries
        if(type == ATA_DEV_TYPE_ERR){
            if(tries == 1){
                tries = 0;
                continue;
            }

            else{
                tries = 1;

                --i;
                continue;
            }
        }

        switch (type)
        {
            case ATA_DEV_TYPE_NONE:
                disks[i] = (disk_t){0, 0, DISK_TYPE_NONE};
                break;
            
            case ATA_DEV_TYPE_PATA:
                disks[i] = (disk_t){1, 0, DISK_TYPE_PATA};
                break; 

            case ATA_DEV_TYPE_PATAPI:
                disks[i] = (disk_t){1, 0, DISK_TYPE_PATAPI};
                break;
    

            default:
                break;
        }
        
    }
};


