#ifndef ATA_H
#define ATA_H

#include "ata_defs.h"
#include "pata.h"
#include "pata_generic.h"





void ATA_write_sectors(uint8_t drive, uint32_t start_sector, uint8_t count, void* buffer, pata_callback_t callback);
void ATA_read_sectors(uint8_t drive, uint32_t start_sector, uint8_t count, void* buffer, pata_callback_t callback);
ata_device_type_t ATA_probe_sync(uint8_t drive, uint16_t* buff);


#endif