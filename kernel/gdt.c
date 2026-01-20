#include "gdt.h"
#include <stdint.h>
#include <stddef.h>

#define GDT_ENTRIES 32

__attribute__((aligned(0x10))) 
gdt_entry_t gdt[GDT_ENTRIES];

gdt_descriptor_t desc;

typedef enum {
    GDT_ACC_ATTR_PRESENT = 0x80,
    GDT_ACC_ATTR_DPL3 = 0x60,
    GDT_ACC_ATTR_DPL0 = 0x0,
    GDT_ACC_ATTR_CODE_DATA = 0x10,
    GDT_ACC_ATTR_EXECUTABLE = 0x08,
    GDT_ACC_ATTR_GROW_DOWN = 0x04,
    GDT_ACC_ATTR_READABLE_WRITABLE = 0x2
} __attribute__ ((packed)) GDT_access_bytes;


typedef enum {
    GDT_FLAG_GRANULARITY = 0x08,
    GDT_FLAG_32_BIT = 0X04,
    GDT_FLAG_64_BIT = 0X02
} __attribute__ ((packed)) GDT_flags;


int8_t init_gdt(){
    for(size_t i = 0; i < GDT_ENTRIES; ++i){
        gdt[i].limit_low = 0;
        gdt[i].base_low = 0;
        gdt[i].base_mid = 0;
        gdt[i].access = 0;
        gdt[i].limit_high_flags = 0;
        gdt[i].base_high = 0;
    }

    // Kernel code
    set_gdt_entry(1, 0xFFFFF, 0, 
        GDT_ACC_ATTR_PRESENT | GDT_ACC_ATTR_DPL0 | GDT_ACC_ATTR_CODE_DATA | GDT_ACC_ATTR_EXECUTABLE | GDT_ACC_ATTR_READABLE_WRITABLE, 
        GDT_FLAG_GRANULARITY | GDT_FLAG_32_BIT
    );

    // Kernel data
    set_gdt_entry(2, 0xFFFFF, 0, 
        GDT_ACC_ATTR_PRESENT | GDT_ACC_ATTR_DPL0 | GDT_ACC_ATTR_CODE_DATA | GDT_ACC_ATTR_READABLE_WRITABLE, 
        GDT_FLAG_GRANULARITY | GDT_FLAG_32_BIT
    );

    // User code
    set_gdt_entry(3, 0xFFFFF, 0, 
        GDT_ACC_ATTR_PRESENT | GDT_ACC_ATTR_DPL3 | GDT_ACC_ATTR_CODE_DATA | GDT_ACC_ATTR_EXECUTABLE, 
        GDT_FLAG_GRANULARITY | GDT_FLAG_32_BIT
    );

    // User data
    set_gdt_entry(4, 0xFFFFF, 0, 
        GDT_ACC_ATTR_PRESENT | GDT_ACC_ATTR_DPL3 | GDT_ACC_ATTR_CODE_DATA | GDT_ACC_ATTR_READABLE_WRITABLE | GDT_ACC_ATTR_READABLE_WRITABLE, 
        GDT_FLAG_GRANULARITY | GDT_FLAG_32_BIT
    );


    

    desc.size = sizeof(gdt) - 1;
    desc.offset = (uint32_t)(&gdt);

    load_gdt(&desc);
}


int8_t set_gdt_entry(int index, uint32_t limit, uint32_t base, uint8_t access_attr, uint8_t flags){
    if(index > GDT_ENTRIES) return -1;

    gdt[index].limit_low = limit & 0xffff;
    gdt[index].base_low = base & 0xffff;
    gdt[index].base_mid = (base >> 16) & 0xff;
    gdt[index].access = access_attr;
    gdt[index].limit_high_flags = ((limit >> 16) & 0xf) | (flags << 4);
    gdt[index].base_high = (base >> 24) & 0xff;
}

