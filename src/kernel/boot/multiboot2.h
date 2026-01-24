#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>
#include <stdbool.h>



typedef enum {
    MB2_TAG_CMD_LINE = 1,
    MB2_TAG_BOOTLDR_NAME = 2,    
    MB2_TAG_MODULES = 3,
    MB2_TAG_MEMINFO = 4,
    MB2_TAG_BOOT_DEV= 5,
    MB2_TAG_MMAP = 6,
    MB2_TAG_VBE_INFO = 7,
    MB2_TAG_FRAMEBUFFER = 8,
    MB2_TAG_ELF_SYMBOLS = 9,
    MB2_TAG_AMP_TABLE = 10,
    MB2_TAG_EFI32_SYS_TABLE_PTR = 11,
    MB2_TAG_EFI64_SYS_TABLE_PTR = 12,
    MB2_TAG_SMBIOS_TABLES = 13,
    MB2_TAG_ACPI_RSDP_V1 = 14,
    MB2_TAG_ACPI_RSDP_V2 = 15,
    MB2_TAG_NETWORK_INFO = 16,
    MB2_TAG_EFI_MMAP = 17,
    MB2_TAG_EFI_SERVICES_ACTIVE = 18,
    MB2_TAG_EFI32_IMAGE_HANDLE_PTR = 19,
    MB2_TAG_EFI64_IMAGE_HANDLE_PTR = 20,
    MB2_TAG_IMAGE_LOAD_BASE_ADDR = 21
} multiboot_tag_type;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint32_t mem_lower;
    uint32_t mem_upper;
} mb2_meminfo_t;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint32_t biosdev;
    uint32_t partition;
    uint32_t sub_partition;
} mb2_boot_dev_t;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    char str[0];
} mb2_cmd_line;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint32_t mod_start;
    uint32_t mod_end;
    char str[0];
} mb2_modules_t;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint16_t num;
    uint16_t entsize;
    uint16_t shndx;
    uint16_t reserved;
} mb2_ELF_symbols_t;


typedef struct __attribute__((packed)) {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} mb2_mmap_entry_t;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
    mb2_mmap_entry_t entries[0];
} mb2_mmap_t;


typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    char str[0];
} mb2_bootldr_name_t;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint16_t version;
    uint16_t cseg;
    uint32_t offset;
    uint16_t cseg_16;
    uint16_t dseg;
    uint16_t flags;
    uint16_t cseg_len;
    uint16_t cseg_16_len;
    uint16_t dseg_len;
} mb2_APM_table_t;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
    uint8_t vbe_control_info[512];
    uint8_t vbe_mode_info[256];
} mb2_VBE_info_t;


typedef struct __attribute__((packed)) {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} mb2_framebuffer_palette_t;

typedef struct __attribute__((packed)) {
    uint32_t framebuffer_palette_num_colors;
    mb2_framebuffer_palette_t framebuffer_palette;
} mb2_framebuffer_color0_t; // if framebuffer_type = 0

typedef struct __attribute__((packed)) {
    uint8_t framebuffer_red_position;
    uint8_t framebuffer_red_mask_size;

    uint8_t framebuffer_green_position;
    uint8_t framebuffer_green_mask_size;

    uint8_t framebuffer_blue_position;
    uint8_t framebuffer_blue_mask_size;
} mb2_framebuffer_color1_t;   // if framebuffer_type = 1

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint64_t framebuffer_addr;
    uint32_t framebufferMB_TAG_BOOT_DEVICE_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;
    uint8_t reserved;
    uint8_t color_info[0];  // dummy for type 0 or 1
} mb2_framebuffer_t;


typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint32_t pointer;
} mb2_EFI32_sys_table_ptr_t;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint64_t pointer;
} mb2_EFI64_sys_table_ptr_t;


typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint8_t major;
    uint8_t minor;
    uint8_t reserved[6];
    uint8_t smbios_tables[0]; 
} mb2_SMBIOS_tables_t;


typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint8_t RSDPv1[0];
} mb2_ACPI_RSDP_v1_t;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint8_t RSDPv2[0];
} mb2_ACPI_RSDP_v2_t;


typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint8_t DHCP_ACK[0];
} mb2_network_info_t;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint32_t descriptor_size;
    uint32_t descriptor_version;
    uint8_t EFI_memory_map;
} mb2_EFI_mmap_t;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
} mb2_EFI_services_active_t;


typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint32_t ptr;
} mb2_EFI32_image_handle_ptr_t;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint64_t ptr;
} mb2_EFI64_image_handle_ptr_t;


typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint32_t load_base_addr;
} mb_image_load_base_addr_t;



typedef union{
    mb2_meminfo_t* meminfo;
    mb2_boot_dev_t* boot_dev;
    mb2_cmd_line* cmd_line;
    mb2_modules_t* modules;
    mb2_ELF_symbols_t* elf_symbols;
    mb2_mmap_t* mmmap;
    mb2_bootldr_name_t* bootldr_name;
    mb2_APM_table_t* APM_table;
    mb2_VBE_info_t* VBE_info;
    mb2_framebuffer_t* framebuffer_info;
    mb2_EFI32_sys_table_ptr_t* EFI32_sys_table_ptr;
    mb2_EFI64_sys_table_ptr_t* EFI64_sys_table_ptr;
    mb2_SMBIOS_tables_t* SMBIOS_tables;
    mb2_ACPI_RSDP_v1_t* ACPI_RSDP_v1;
    mb2_ACPI_RSDP_v2_t* ACPI_RSDP_v2;
    mb2_network_info_t* network_info;
    mb2_EFI_mmap_t* EFI_memmap;
    mb2_EFI_services_active_t* EFI_services_active;
    mb2_EFI32_image_handle_ptr_t* EFI32_image_hand_ptr;
    mb2_EFI64_image_handle_ptr_t* EFI64_image_hand_ptr;
    mb_image_load_base_addr_t* image_base_addr;
    void* raw;
} mb_tag_ptr;


mb_tag_ptr mbi2_get_tag(uint8_t* mbi, multiboot_tag_type tag);
bool mb2_is_magic(uint32_t magic);

#endif