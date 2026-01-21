#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../drivers/partition.h"

#define FAT32_SIGNATURE         0xAA55
#define FAT32_CLUSTER_FREE      0x00000000
#define FAT32_CLUSTER_BAD       0x0FFFFFF7
#define FAT32_CLUSTER_EOC_MIN   0x0FFFFFF8
#define FAT32_CLUSTER_EOC_MAX   0x0FFFFFFF

#define DIR_ATTR_READ_ONLY      0x01
#define DIR_ATTR_HIDDEN         0x02
#define DIR_ATTR_SYSTEM         0x04
#define DIR_ATTR_VOLUME_ID      0x08
#define DIR_ATTR_DIRECTORY      0x10
#define DIR_ATTR_ARCHIVE        0x20
#define DIR_ATTR_LONG_NAME      (DIR_ATTR_READ_ONLY | DIR_ATTR_HIDDEN | DIR_ATTR_SYSTEM | DIR_ATTR_VOLUME_ID)

typedef struct {
    uint8_t jump_code[3];           // Alway to be set to EB 3C 90
    char oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_count;              // Usually 2
    uint16_t root_entries;          // 0 for FAT32
    uint16_t total_sectors_16;      // 0 for FAT32
    uint8_t media_descriptor;
    uint16_t sectors_per_fat_16;    // 0 for FAT32
    uint16_t sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;        // i.e. the LBA of the beginning of the partition.
    uint32_t total_sectors_32;
    
    // FAT32 specific
    uint32_t sectors_per_fat_32;
    uint16_t flags;
    uint16_t version;           // The high byte is the major version and the low byte is the minor version.
    uint32_t root_cluster;
    uint16_t fsinfo_sector;
    uint16_t backup_boot_sector;
    uint8_t reserved[12];
    uint8_t drive_number;
    uint8_t reserved1;
    uint8_t signature;          // must be 0x28 or 0x29
    uint32_t serial_number;     // Can be ignored for sake of simplicity
    char volume_label[11];      // Padded with spaces
    char filesystem_type[8];    // System identifier string. Always "FAT32   ". The spec says never to trust the contents of this string for any use. 
    uint8_t boot_code[420];
    uint16_t boot_signature;    // 0xAA55
} __attribute__((packed)) fat32_boot_record_t;

typedef struct {
    char name[8];                   
    char extension[3];
    uint8_t attributes;                 // READ_ONLY=0x01 HIDDEN=0x02 SYSTEM=0x04 VOLUME_ID=0x08 DIRECTORY=0x10 ARCHIVE=0x20 LFN=READ_ONLY|HIDDEN|SYSTEM|VOLUME_ID 
    uint8_t reserved;
    uint8_t creation_time_hundredths;   // Can be safely ignored (Ubuntu already does, so ¯\_(ツ)_/¯)
    uint16_t creation_time;             // Hour = 5bits; Minutes = 6bits; Seconds = 5bits. Multiply seconds by 2 for whatever reason
    uint16_t creation_date;             // Year = 7bits; Month = 4bits; Day = 5bits
    uint16_t last_access_date;          // Same format as creation
    uint16_t cluster_high;              
    uint16_t modification_time;         // Same format as modification time
    uint16_t modification_date;         // Same format as modification time
    uint16_t cluster_low;               
    uint32_t file_size;                 // In bytes
} __attribute__((packed)) fat32_dir_entry_t;


typedef struct {
    partition_t partition;
    fat32_boot_record_t boot_sector;
    uint32_t fat_start_sector;
    uint32_t data_start_sector;
    uint32_t cluster_size_bytes;
    uint32_t* fat_cache;          // Cache for FAT table
    bool fat_cache_dirty;
} fat32_fs_t;

typedef struct {
    fat32_fs_t* fs;
    uint32_t current_cluster; 
    uint32_t position;          // Current readbug position
    uint32_t size;              // Size in bytes
    bool is_directory;
    char name[256];
} fat32_dir_t;

int8_t fat32_mount(partition_t* partition, fat32_fs_t* fs);
void fat32_unmount(fat32_fs_t* fs);
int fat32_readdir(fat32_dir_t* dir, fat32_dir_entry_t* entry);
