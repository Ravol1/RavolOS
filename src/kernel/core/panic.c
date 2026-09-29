#include "panic.h"

#include <stdint.h>

#include "drivers/video/video.h"
#include "arch/i386/gdt/tss.h"


extern char _stack_guard[];
extern char _stack_bottom[];
extern char _stack_top[];


void kernel_panic_handler(){
    asm volatile("cli");

    uint32_t crashed_esp = cs_tss.esp;
    uint32_t crashed_eip = cs_tss.eip;

    clear_screen();
    kprintf("KENREL PANIC! KENREL PANIC! KENREL PANIC!\n\n");

    if (crashed_esp >= (uint32_t)_stack_guard && crashed_esp < (uint32_t)_stack_bottom){
        kprintf("Reason: KERNEL STACK OVERFLOW\n");
    }
    else{
        kprintf("Reason: DOUBLE FAULT (Generic)\n");
    }

    kprintf("Crashed at EIP: %p\n", crashed_eip);
    kprintf("Crashed with ESP: %p\n", crashed_esp);

    kprintf("\nKENREL PANIC! KENREL PANIC! KENREL PANIC!\n\n");

    kprintf("\n\nReboot is the only option.\n");

    while(1) asm volatile("hlt");
}