#pragma once

#include <stdint.h>
#include "../drivers/video.h"

#define KERNEL_CODE_SELECTOR 0x08


typedef struct {
    uint16_t isr_low;           // Lower 16 bits of ISR's address
    uint16_t kernel_cs;         // The GDT segment selector that the CPU will load into CS before calling the ISR
    uint8_t reserved;           // Set to 0
    uint8_t attributes;         // Lower 4: Gate type; bit 4: 0; bits 5-6: DLP; bit 7: P
    uint16_t isr_high;          // Higher 16 bits of ISR's address
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;     // IDT size -1   
    uint32_t base;      // Linear address of IDT (not the physical address, paging applies).
} __attribute__((packed)) idtr_t;


typedef struct {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
} __attribute__((packed)) syscall_params_t;


void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_init(void);

extern void load_idt(idtr_t* descriptor);

