#ifndef PATA_GENERIC_H
#define PATA_GENERIC_H

#include <stdint.h>

#include "ata_defs.h"

#define PATAPI_mid 0x14
#define PATAPI_hi 0xEB


enum pata_port{
    PATA_PRIMARY = 0x1F0,
    PATA_SECONDARY = 0x170
};

enum pata_port_ofst {
    PATA_OFST_DATA_REG = 0x00,
    PATA_OFST_ERROR_REG = 0x01,
    PATA_OFST_FEATURE_REG = 0x010,
    PATA_OFST_SECTOR_COUNT_REG = 0x02,
    PATA_OFST_LBAlo_REG = 0x03,
    PATA_OFST_LBAmid_REG = 0x04,
    PATA_OFST_LBAhi_REG = 0x05,
    PATA_OFST_DRIVE_REG = 0x06,
    PATA_OFST_STATUS_REG = 0x07,
    PATA_OFST_ALT_STATUS_REG = 0x206,
    PATA_OFST_DEVICE_CONTROL_REG = 0x206,
    PATA_OFST_COMMAND_REG = 0x07,
};



typedef enum {
    PATA_OP_NONE = 0,
    PATA_OP_WRITE,
    PATA_OP_READ,
    PATA_OP_IDENTIFY_SYNC,
    PATA_OP_IDENTIFY_ASYNC
} pata_operation_type;

typedef enum {
    PATA_OPSTATUS_IDLE = 0,
    PATA_OPSTATUS_PENDING,
    PATA_OPSTATUS_DONE,
    PATA_OPSTATUS_ERROR
} pata_operation_status;



// COMPILER, JUST SHUT UP AND LET ME DO MY THINGS!
typedef struct pata_operation pata_operation_t;
typedef void (*pata_callback_t)(pata_operation_t*);

struct pata_operation{
    uint16_t* buffer;
    pata_operation_type operation_type;
    uint32_t start_lba;
    uint32_t total_sectors;     
    uint32_t current_sector;        // 0 based 
    pata_operation_status operation_status;
    uint8_t error_code;
    pata_callback_t on_done;
};
// COMPILER, JUST SHUT UP AND LET ME DO MY THINGS!



extern pata_operation_t pata_primary_op;
extern pata_operation_t pata_secondary_op;



ata_error_t PATA_reset_soft(uint8_t port);
ata_error_t PATA_reset_hard(uint8_t drive);

uint8_t PATA_get_status(enum pata_port port, bool alt);

int PATA_read_sector(void* buffer, enum pata_port port);

int PATA_primary_irq_handler(void);
void PATA_read_pio(uint8_t drive, uint32_t start_sector, uint8_t count, void* buffer, pata_callback_t callback);
void PATA_write_pio(uint8_t drive, uint32_t start_sector, uint8_t count, void* buffer, pata_callback_t callback);

#endif