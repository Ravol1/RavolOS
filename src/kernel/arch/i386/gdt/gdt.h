#include <stdint.h>

typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t limit_high_flags;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct {
    uint16_t size;
    uint32_t offset;
} __attribute__ ((packed)) gdt_descriptor_t;

int8_t init_gdt();
int8_t set_gdt_entry(int index, uint32_t limit, uint32_t base, uint8_t flags, uint8_t access_attr);

extern void load_gdt(gdt_descriptor_t* desc);