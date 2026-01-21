#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "arch/i386/gdt/gdt.h"
#include "arch/i386/interrupt/idt.h"
#include "arch/i386/io/io.h"
#include "core/mm/memory.h"
#include "core/timer/timer.h"
#include "core/syscall/syscall.h"
#include "drivers/video/video.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/keyboard/keyboard_mapping.h"




#define MAGIC_MB1 0x2BADB002
#define MAGIC_MB2 0x36D76289



extern void spam_syscall();


typedef struct __attribute__((packed)) multiboot_info {
    uint32_t flags;         // quali campi sono validi
    uint32_t mem_lower;     // memoria inferiore (KB)
    uint32_t mem_upper;     // memoria superiore (KB)
    uint32_t boot_device;   // BIOS disk device da cui si è bootato
    uint32_t cmdline;       // puntatore a stringa cmdline
    uint32_t mods_count;    // numero di moduli caricati
    uint32_t mods_addr;     // puntatore array moduli
    uint32_t syms[4];       // simboli a seconda formato a.out o ELF
    uint32_t mmap_length;   // lunghezza della memory map
    uint32_t mmap_addr;     // puntatore alla memory map
    uint32_t drives_length; // lunghezza della lista dei drive
    uint32_t drives_addr;   // puntatore alla lista dei drive
    uint32_t config_table;  // puntatore alla config table (opzionale)
    uint32_t boot_loader_name; // stringa con il nome del bootloader
    uint32_t apm_table;     // puntatore alla APM table
    uint32_t vbe_control_info; // VBE info
    uint32_t vbe_mode_info;    // VBE mode info
    uint16_t vbe_mode;         // VBE mode
    uint16_t vbe_interface_seg; 
    uint16_t vbe_interface_off; 
    uint16_t vbe_interface_len; 
} multiboot_info_t;



void init(){
    init_gdt();
    set_system_clock(TICK_PER_SECOND);
    idt_init();
    init_mem();
    init_video();

}


extern int main(uint32_t magic, multiboot_info_t* mbi){
    if (magic == MAGIC_MB1) {
        printf("Booted with Multiboot 1.\n");
    }

    else if(magic== MAGIC_MB2){
        printf("Booted with Multiboot 2.\n");
    }

    else{
        printf("No multiboot detected.\n");
        return -1;
    }
    init();    
    
    //spam_syscall();
    
    while(1){
        if(keyboard_map[KEY_A])
            outw(0x4004, 0x3400);       // Shutdown (QEMU specific)

        
        asm volatile("hlt");
    }
    
/*
    syscall_regs_caller* regs;

    memset(regs, 0, sizeof(syscall_regs_caller));
    regs->eax = (uint32_t)str;

    system_call(regs);

    while(1);
*/
}





