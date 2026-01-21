#include "interrupt.h"
#include "drivers/keyboard/keyboard.h"
#include "stdint.h"
#include "drivers/video/video.h"
#include "drivers/x86/i386/pic/pic.h"
#include "core/syscall/syscall.h"
#include "core/timer/timer.h"


void exception_handler(registers_t *reg){
    vga_print("Exception %u occurred with code %u\n", reg->int_no, reg->err_code);
    while(1);
}


void interrupt_generic(registers_t *reg){
    vga_print("Interrupt %u called\n", reg->int_no);
}

void irq_handler(registers_t *reg){
    uint32_t int_no = reg->int_no;

    switch(int_no){
            case 32:
                tick_update();

                // schedule once implemented
                
                break;
            case 33:
                uint16_t scancode = get_keyboard_code();
                set_keyboard_map(scancode);
                break;
            default:
                interrupt_generic(reg);
                break;
        }
        PIC_sendEOI(int_no - 32);
}

void user_handler(registers_t *reg)
{
    uint32_t int_no = reg->int_no;

    switch (int_no)
        {
            case 0x80:          // Syscall handler  

                syscall_args_t args = {
                    .syscall_num = reg->eax,
                    .args = {
                        reg->ebx,
                        reg->ecx,
                        reg->edx,
                        reg->esi,
                        reg->edi,
                        reg->ebp
                    }
                };

                syscall_handler(&args);
                break;
        default:
            interrupt_generic(reg);
            break;
        }
}

void interrupt_handler(registers_t *reg){
    
    uint32_t int_no = reg->int_no;
    
    if(int_no < 32){
        exception_handler(reg);
    }

    else if (int_no < 48){
        irq_handler(reg);
    }

    else{
        user_handler(reg);
    }
}