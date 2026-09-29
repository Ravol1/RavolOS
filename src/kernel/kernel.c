#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "arch/i386/gdt/gdt.h"
#include "arch/i386/interrupt/idt.h"
#include "arch/i386/memory/paging/paging.h"
#include "boot/multiboot2.h"
#include "core/memory/memory.h"
#include "core/timer/timer.h"
#include "core/syscall/syscall.h"
#include "drivers/io/io.h"
#include "drivers/video/video.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/keyboard/keyboard_mapping.h"
#include "drivers/storage/disk.h"
#include "drivers/tests/ata_pio.h"



extern void spam_syscall();
extern void stack_overflow();


void init(){

}


extern int main(uint32_t magic, uint8_t* mbi){
    video_init();

    if(!mb2_is_magic(magic)){
        kprintf("No multiboot2 detected");
    }

    mem_init_status status = mem_init(mbi);

    switch (status)
    {
    case MEM_INIT_OK:
        kprintf("MEM OK.\n");
        break;
    case MEM_INIT_NO_MB2_MMAP:
        kprintf("NO MMAP.\n");
        break;

    case MEM_INIT_MEM_TRUNC:
        kprintf("MEM TRUNC.\n");
        break;
    default:
        kprintf("MEM UNKNOWN ERROR.\n");
        break;
    }

    page_init();
    gdt_init();

    set_system_clock(TICK_PER_SECOND);
    idt_init();

    
    spam_syscall();
}    








