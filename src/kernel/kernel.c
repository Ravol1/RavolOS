#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "arch/i386/gdt/gdt.h"
#include "arch/i386/interrupt/idt.h"
#include "arch/i386/io/io.h"
#include "arch/i386/memory/paging/paging.h"
#include "boot/multiboot2.h"
#include "core/memory/memory.h"
#include "core/timer/timer.h"
#include "core/syscall/syscall.h"
#include "drivers/video/video.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/keyboard/keyboard_mapping.h"
#include "drivers/storage/disk.h"



extern void spam_syscall();
extern void stack_overflow();


void init(){

}


extern int main(uint32_t magic, uint8_t* mbi){
    vga_init();    

    if(!mb2_is_magic(magic)){
        vga_print("No multiboot2 detected");
    }

    mem_init_status status = mem_init(mbi);

    switch (status)
    {
    case MEM_INIT_OK:
        vga_print("MEM OK.\n");
        break;
    case MEM_INIT_NO_MB2_MMAP:
        vga_print("NO MMAP.\n");
        break;

    case MEM_INIT_MEM_TRUNC:
        vga_print("MEM TRUNC.\n");
        break;
    default:
        vga_print("MEM UNKNOWN ERROR.\n");
        break;
    }

    page_init();
    gdt_init();

    set_system_clock(TICK_PER_SECOND);
    idt_init();
    
    while(1){
        if(keyboard_map[KEY_SPACEBAR]) break;
        asm volatile("hlt");
    }

    stack_overflow();


    while(1){
        asm volatile("hlt");
    }

    // spam_syscall();
    
    uint8_t* buff;
    
    buff = malloc(2048);
    memset(buff, 0, 2048);

    uint8_t* read_buff;
    read_buff = malloc(2048);
    memset(read_buff, 'A', 2048);

    ATA_write_sectors(0, 1, 4, buff);

    vga_print("\nDONE WRITING\n");

    ATA_read_sectors(0, 1, 4, read_buff);


        
    for(size_t i = 0; i<2048; ++i){
        vga_print("%c", (char)read_buff[i]);
    }
    
    vga_print("\nDONE");
}    








