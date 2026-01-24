#include "panic.h"
#include "drivers/video/video.h"
#include "arch/i386/gdt/tss.h"


extern char _stack_guard[];
extern char _stack_bottom[];
extern char _stack_top[];


void kernel_panic_handler(){
    asm volatile("cli");

    uint32_t crashed_esp = cs_tss.esp;
    uint32_t crashed_eip = cs_tss.eip;

    vga_clear_screen();
    vga_print("KENREL PANIC! KENREL PANIC! KENREL PANIC!\n\n");

    if (crashed_esp >= (uint32_t)_stack_guard && crashed_esp < (uint32_t)_stack_bottom){
        vga_print("Reason: KERNEL STACK OVERFLOW\n");
    }
    else{
        vga_print("Reason: DOUBLE FAULT (Generic)\n");
    }

    vga_print("Crashed at EIP: %p\n", crashed_eip);
    vga_print("Crashed with ESP: %p\n", crashed_esp);

    vga_print("\nKENREL PANIC! KENREL PANIC! KENREL PANIC!\n\n");

    vga_print("\n\nReboot is the only option.\n");

    while(1) asm volatile("hlt");
}