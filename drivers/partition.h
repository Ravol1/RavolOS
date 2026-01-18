#pragma once

#include <stdint.h>

typedef enum {
    PARTION_TYPE_FAT32_CHS = 0x0B,
    PARTION_TYPE_FAT32_LBA = 0x0C
} Partition_Type;

typedef struct {
    uint8_t status;           // 0x80 = bootable, 0x00 = non bootable
    uint8_t chs_start[3];     // CHS start address
    uint8_t type;             // Partition type
    uint8_t chs_end[3];       // CHS end address
    uint32_t lba_start;       // LBA start sector
    uint32_t sector_count;    // Number of sectors
} __attribute__((packed)) partition_entry_t;

typedef struct {
    uint8_t boot_code[446];
    partition_entry_t partitions[4];
    uint16_t signature;       // 0xAA55
} __attribute__((packed)) mbr_t;

typedef struct {
    uint8_t drive;
    uint8_t partition_index;
    uint32_t start_sector;
    uint32_t sector_count;
    uint8_t type;
} partition_t;

int8_t partition_read_mbr(uint8_t drive, mbr_t* mbr);
int8_t partition_enumerate(uint8_t drive, partition_t* partitions, int max_partitions);

