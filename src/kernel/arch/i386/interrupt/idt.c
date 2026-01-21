#include <stdbool.h>

#include "idt.h"
#include "drivers/x86/i386/pic/pic.h"
#include "arch/i386/io/io.h"

#define IDT_ENTRIES 256
#define KERNEL_CODE_SEGMENT 0x08

		/* End-of-interrupt command code */

static bool vectors[IDT_ENTRIES];

extern void exception0(void);
extern void exception1(void);
extern void exception2(void);
extern void exception3(void);
extern void exception4(void);
extern void exception5(void);
extern void exception6(void);
extern void exception7(void);
extern void exception8(void);
extern void exception9(void);
extern void exception10(void);
extern void exception11(void);
extern void exception12(void);
extern void exception13(void);
extern void exception14(void);
extern void exception15(void);
extern void exception16(void);
extern void exception17(void);
extern void exception18(void);
extern void exception19(void);
extern void exception20(void);
extern void exception21(void);
extern void exception22(void);
extern void exception23(void);
extern void exception24(void);
extern void exception25(void);
extern void exception26(void);
extern void exception27(void);
extern void exception28(void);
extern void exception29(void);
extern void exception30(void);
extern void exception31(void);

static void (*ex_handlers[32])(void) = {
    exception0,  exception1,  exception2,  exception3,
    exception4,  exception5,  exception6,  exception7,
    exception8,  exception9,  exception10, exception11,
    exception12, exception13, exception14, exception15,
    exception16, exception17, exception18, exception19,
    exception20, exception21, exception22, exception23,
    exception24, exception25, exception26, exception27,
    exception28, exception29, exception30, exception31
};


extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

static void (*irq_handlers[16])(void) = {
    irq0, irq1, irq2, irq3,
    irq4, irq5, irq6, irq7,
    irq8, irq9, irq10, irq11,
    irq12, irq13, irq14, irq15
};


extern void int128(void);

static void (*user_handlers [1])(void) = {
    int128
};



__attribute__((aligned(0x10))) 
static idt_entry_t idt[IDT_ENTRIES];

static idtr_t idtr;


typedef enum {
    IDT_FLAG_PRESENT = 0x80,
    IDT_FLAG_DPL_0 = 0x00,      
    IDT_FLAG_DPL_3 = 0x60,      

} IDT_entry_types;

typedef enum {
    IDT_FLAG_TASK_GATE = 0x5,
    IDT_FLAG_INT_GATE_16 = 0x06,
    IDT_FLAG_INT_TRAP_16 = 0x07,
    IDT_FLAG_INT_GATE = 0x0E, 
    IDT_FLAG_TRAP_GATE = 0x0F
} IDT_gate_type;



void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags){
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low = (uint32_t)isr & 0xffff;
    descriptor->kernel_cs = KERNEL_CODE_SELECTOR;
    descriptor->attributes = flags;
    descriptor->isr_high = (uint32_t)isr >> 16;
    descriptor->reserved = 0;
}


void idt_init(){
    idtr.base = (uint32_t)&idt[0];
    idtr.limit = ((uint16_t)sizeof(idt_entry_t)) * IDT_ENTRIES - 1;


    // Set exceptions handlers
    for(uint8_t vector = 0; vector < 32; ++vector){
        idt_set_descriptor(vector, ex_handlers[vector], IDT_FLAG_TRAP_GATE | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
        vectors[vector] = true;
    }


    // Set irq handlers
    for(uint8_t vector = 32; vector < 48; ++vector){
        idt_set_descriptor(vector, irq_handlers[vector - 32], IDT_FLAG_INT_GATE | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
        vectors[vector] = true;
    }

    idt_set_descriptor(0x80, int128, IDT_FLAG_INT_GATE | IDT_FLAG_DPL_3 | IDT_FLAG_PRESENT);
    vectors[128] = true;


    remap_pic(0x20, 0x28);
    //disable_pic();
    load_idt(&idtr);

    //while(1);
}






