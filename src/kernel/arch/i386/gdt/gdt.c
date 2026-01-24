#include "gdt.h"
#include "tss.h"
#include <stdint.h>
#include <stddef.h>

#define GDT_ENTRIES 32

gdt_entry_t gdt[GDT_ENTRIES] __attribute__((aligned(0x10)));

gdt_descriptor_t desc;

typedef enum {
    GDT_ACC_NORM_PRESENT = 0x80,
    
    GDT_ACC_NORM_DPL3 = 0x60,
    GDT_ACC_NORM_DPL0 = 0x0,
    
    GDT_ACC_NORM_SYSTEM_SEG = 0x00,
    GDT_ACC_NORM_CODE_DATA_SEG = 0x10,

    GDT_ACC_NORM_EXECUTABLE = 0x08,
    GDT_ACC_NORM_GROW_DOWN = 0x04,
    GDT_ACC_NORM_READWRITE = 0x02,
    GDT_ACC_NORM_ACCESSED = 0x01
} GDT_access_byte_norm;

typedef enum {
    GDT_ACC_TSS_TYPE_AVAIL16 = 0x01,
    GDT_ACC_TSS_TYPE_LDT = 0x02,
    GDT_ACC_TSS_TYPE_BUSY16 = 0x03,
    GDT_ACC_TSS_TYPE_AVAIL32 = 0x09,
    GDT_ACC_TSS_TYPE_BUSY = 0x0B,

    GDT_ACC_TSS_SYSTEM_SEG = 0x00,
    GDT_ACC_TSS_CODE_DATA_SEG = 0x10,

    GDT_ACC_TSS_DPL3 = 0x60,
    GDT_ACC_TSS_DPL0 = 0x00,

    GDT_ACC_TSS_PRESENT = 0x80,
} GDT_access_byte_tss;


typedef enum {
    GDT_FLAG_GRANULARITY = 0x08,
    GDT_FLAG_32_BIT = 0x04,
    GDT_FLAG_64_BIT = 0x02,
} GDT_flags;


extern void load_gdt(gdt_descriptor_t* desc);
extern void load_tr(uint32_t selector);


int8_t gdt_set_entry(int index, uint32_t limit, uint32_t base, GDT_access_byte_norm access_attr, GDT_flags flags){
    if(index > GDT_ENTRIES) return -1;

    gdt[index].limit_low = limit & 0xffff;
    gdt[index].base_low = base & 0xffff;
    gdt[index].base_mid = (base >> 16) & 0xff;
    gdt[index].access = access_attr;
    gdt[index].limit_high_flags = ((limit >> 16) & 0xf) | (flags << 4);
    gdt[index].base_high = (base >> 24) & 0xff;
}

int8_t gdt_set_tss(int index, uint32_t limit, uint32_t base, GDT_access_byte_tss access_attr){
    if(index > GDT_ENTRIES) return -1;

    gdt[index].limit_low = limit & 0xffff;
    gdt[index].base_low = base & 0xffff;
    gdt[index].base_mid = (base >> 16) & 0xff;
    gdt[index].access = access_attr;
    gdt[index].limit_high_flags = ((limit >> 16) & 0xf);
    gdt[index].base_high = (base >> 24) & 0xff;
}


int8_t gdt_init(){
    for(size_t i = 0; i < GDT_ENTRIES; ++i){
        gdt[i].limit_low = 0;
        gdt[i].base_low = 0;
        gdt[i].base_mid = 0;
        gdt[i].access = 0;
        gdt[i].limit_high_flags = 0;
        gdt[i].base_high = 0;
    }

    // Kernel code
    gdt_set_entry(1, 0xFFFFF, 0, 
        GDT_ACC_NORM_PRESENT | GDT_ACC_NORM_DPL0 | GDT_ACC_NORM_CODE_DATA_SEG | GDT_ACC_NORM_EXECUTABLE, 
        GDT_FLAG_GRANULARITY | GDT_FLAG_32_BIT
    );

    // Kernel data
    gdt_set_entry(2, 0xFFFFF, 0, 
        GDT_ACC_NORM_PRESENT | GDT_ACC_NORM_DPL0 | GDT_ACC_NORM_CODE_DATA_SEG | GDT_ACC_NORM_READWRITE, 
        GDT_FLAG_GRANULARITY | GDT_FLAG_32_BIT
    );

    // User code
    gdt_set_entry(3, 0xFFFFF, 0, 
        GDT_ACC_NORM_PRESENT | GDT_ACC_NORM_DPL3 | GDT_ACC_NORM_CODE_DATA_SEG | GDT_ACC_NORM_EXECUTABLE, 
        GDT_FLAG_GRANULARITY | GDT_FLAG_32_BIT
    );

    // User data
    gdt_set_entry(4, 0xFFFFF, 0, 
        GDT_ACC_NORM_PRESENT | GDT_ACC_NORM_DPL3 | GDT_ACC_NORM_CODE_DATA_SEG | GDT_ACC_NORM_READWRITE, 
        GDT_FLAG_GRANULARITY | GDT_FLAG_32_BIT
    );

    // General context switch
    gdt_set_tss(
        5, 
        sizeof(cs_tss) - 1, 
        (uint32_t)&cs_tss,
        GDT_ACC_TSS_TYPE_AVAIL32 | GDT_ACC_TSS_DPL0 | GDT_ACC_TSS_SYSTEM_SEG | GDT_ACC_TSS_PRESENT 
    );

    // Emergency double fault context switch
    gdt_set_tss(
        6, 
        sizeof(df_tss) - 1, 
        (uint32_t)&df_tss,
        GDT_ACC_TSS_TYPE_AVAIL32 | GDT_ACC_TSS_DPL0 | GDT_ACC_TSS_SYSTEM_SEG | GDT_ACC_TSS_PRESENT
    );


    desc.size = sizeof(gdt) - 1;
    desc.offset = (uint32_t)(&gdt);

    tss_init();

    load_gdt(&desc);
    load_tr(5 * 0x08);  // Main (Context Switch) task gate, gdt entry number 5
}


