#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "memory.h"
#include "idt.h"
#include "timer.h"
#include "../drivers/video.h"
#include "../drivers/keyboard.h"
#include "../drivers/kbd_layouts.h"
#include "../not_kernel/system_call.h"



char* str = "Hello world!";

extern void spam_syscall();

void init(){
    idt_init();
    init_mem();
    init_video();
}


extern int main(){
    //init();    
    //set_pit_frq(10000);
    //spam_syscall();
    printf("Hello from GRUB, world!");
    while(1);
    
/*
    syscall_regs_caller* regs;

    memset(regs, 0, sizeof(syscall_regs_caller));
    regs->eax = (uint32_t)str;

    system_call(regs);

    while(1);
*/
}





