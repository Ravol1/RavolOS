#pragma once

#define ATA_PRIMARY_IO  0x1F0
#define ATA_PRIMARY_CTRL 0x3F6
#define ATA_SECTOR_SIZE 512

#define INVALID_SECTOR 0xFFFFFFFF


int disk_init(void);

int8_t ATA_read_sectors(uint8_t drive, uint32_t start_sector, uint32_t count, void* buffer);
