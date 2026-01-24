#include <stdint.h>
#include "core/memory/memory.h"
#include "core/panic.h"
#include "tss.h"

#include "drivers/video/video.h"



volatile uint8_t double_fault_stack[4096];

tss_t cs_tss __attribute__((aligned(16)));
tss_t df_tss __attribute__((aligned(16)));


extern void set_df_tss_cr3(uint32_t* cr3);

void tss_init(){
    memset(&df_tss, 0, sizeof(tss_t));

    df_tss.esp = (uint32_t)&double_fault_stack[4096];
    df_tss.ebp = (uint32_t)&double_fault_stack[4096];

    df_tss.eip = (uint32_t)kernel_panic_handler;

    df_tss.cs = 0x08;
    df_tss.ds = 0x10;
    df_tss.es = 0x10;
    df_tss.fs = 0x10;
    df_tss.gs = 0x10;
    df_tss.ss = 0x10;

    uint32_t current_cr3;
    asm volatile("mov %%cr3, %0" : "=r"(current_cr3));
    df_tss.cr3 = current_cr3;
    
    df_tss.iopb = sizeof(tss_t) << 16;
    

    cs_tss.esp = (uint32_t)&double_fault_stack[4096];
    cs_tss.ebp = (uint32_t)&double_fault_stack[4096];
    

    cs_tss.cs = 0x08;
    cs_tss.ds = 0x10;
    cs_tss.es = 0x10;
    cs_tss.fs = 0x10;
    cs_tss.gs = 0x10;
    cs_tss.ss = 0x10;


    cs_tss.cr3 = current_cr3;
    
    cs_tss.iopb = sizeof(tss_t) << 16;
}