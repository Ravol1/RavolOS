#ifndef ATA_DEFS_H
#define ATA_DEFS_H


typedef enum {
    ATA_COMMAND_NOP = 0x00,

    ATA_COMMAND_REQUEST_EXTENDED_ERROR = 0x03,
    ATA_COMMAND_DEVICE_RESET = 0x08,

    ATA_COMMAND_READ_SECTORS = 0x20,
    ATA_COMMAND_READ_SECTORS_EXT = 0x24,
    ATA_COMMAND_READ_DMA_EXT = 0x25,
    ATA_COMMAND_READ_DMA = 0xC8,

    ATA_COMMAND_WRITE_SECTORS = 0x30,
    ATA_COMMAND_WRITE_SECTORS_EXT = 0x34,
    ATA_COMMAND_WRITE_DMA_EXT = 0x35,
    ATA_COMMAND_WRITE_DMA = 0xCA,

    ATA_COMMAND_DEVICE_DIAGNOSTIC = 0x90,
    
    ATA_COMMAND_ERASE_SECTORS = 0xC0,
    ATA_COMMAND_STANDBY_IMMEDIATE = 0xE0,
    ATA_COMMAND_IDLE_IMMEDIATE = 0xE1,
    ATA_COMMAND_STANDBY = 0xE2,
    ATA_COMMAND_IDLE = 0xE3,
    
    ATA_COMMAND_FLUSH_CACHE = 0xE7,
    ATA_COMMAND_FLUSH_CACHE_EXT = 0xEA,
    
    ATA_COMMAND_IDENTIFY = 0xEC,
    ATA_COMMAND_IDENTIFY_PACKET = 0xA1
} ata_command;


/**
 * @brief PATA Error Codes
 * * Uses a bitmask layout to combine hardware-level errors with 
 * driver-level state. 
 * * - Bits [0:7]: Standard ATA Error Register mapping (mirrors hardware).
 * - Bits [8:15]: Custom driver/logic errors (timeouts, floating bus, etc.).
 * * Note: If a hardware error (bits 0-7) is present, ATA_ERR_GEN must also 
 * be set to indicate the lower bits are valid.
 * * @note DRIVE FAULT logic: A Drive Fault is identified when ATA_ERR_GEN 
 * is set, but all lower hardware bits (0-7) remain zero.
 */
typedef enum ata_error {
    /* --- Standard ATA Error Register Bits (Lower Byte) --- */
    ATA_ERR_AMNF = 0x01,
    ATA_ERR_TKZNF = 0x02,
    ATA_ERR_ABRT = 0x04,
    ATA_ERR_MCR = 0x08,
    ATA_ERR_IDNF = 0x10,
    ATA_ERR_MC = 0x20,
    ATA_ERR_UNC = 0x40,
    ATA_ERR_BBK = 0x80,

    /* --- Custom Driver Status (Upper Byte) --- */
    ATA_ERR_GEN = 0x0100,
    ATA_ERR_FLOAT = 0x0200,
    ATA_ERR_TIME = 0x0400
} ata_error_t;

typedef enum ata_status {
    ATA_STATUS_ERR = 0x01,
    ATA_STATUS_IDX = 0x02,
    ATA_STATUS_CORR = 0x04,
    ATA_STATUS_DRQ = 0x08,
    ATA_STATUS_SRV = 0x10,
    ATA_STATUS_DF = 0x20,
    ATA_STATUS_RDY = 0x40,
    ATA_STATUS_BSY = 0x80
} ata_status_t;

typedef enum {
    ATA_DEV_TYPE_ERR = 0,
    ATA_DEV_TYPE_PATA,
    ATA_DEV_TYPE_PATAPI,
    ATA_DEV_TYPE_SATA,
    ATA_DEV_TYPE_NONE       
} ata_device_type_t;


#endif