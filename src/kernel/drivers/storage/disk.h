#ifndef DISK_H
#define DISK_H

#include <stdbool.h>
#include <stdint.h>

#include "drivers/storage/ata/pata.h"

typedef enum disk_type{
    DISK_TYPE_NONE = 0,
    DISK_TYPE_ERR = -1,
    DISK_TYPE_PATA,
    DISK_TYPE_PATAPI,
    DISK_TYPE_SATA,
    DISK_TYPE_USB
} disk_type_t;

typedef struct disk_caps {
    uint32_t lba48_supported : 1;
    uint32_t dma_supported   : 1;
    uint32_t ncq_supported   : 1;
    uint32_t removable       : 1;
    uint32_t reserved        : 28;
} disk_caps_t;


typedef struct disk disk_t; // Forward declration because the compiler is a bitch


typedef struct disk_operations {
    int (*read_sectors)(disk_t* d, uint64_t lba, uint32_t count, void* buffer);
    int (*write_sectors)(disk_t* d, uint64_t lba, uint32_t count, void* buffer);
    void (*flush)(disk_t* d);
} disk_ops_t;


typedef struct disk {
    // --- Common ---
    bool present;
    bool accessed;
    disk_type_t type;
    disk_caps_t caps;

    uint64_t length_sectors;
    uint32_t sector_size;

    char serial[21];
    char firmware[9];
    char model[41];

    // --- Protocol specific ---
    union {
        struct  {

        } pata;

        struct  {
            
            uint8_t packet_size;
            uint8_t device_type;
        } patapi;

        struct {

            uint8_t queue_depth;  
        } achi;
        
        
    } spec;
    
    disk_ops_t* ops;
    uint32_t version;
} disk_t;



int disk_read(uint64_t start, uint64_t count, uint16_t* buffer);
void disk_init(void);

#endif