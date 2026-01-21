#include "stdint.h"
#include "syscall.h"
#include "core/timer/timer.h"
#include "drivers/video/video.h"


void syscall_handler(syscall_args_t* args){
    vga_print("Syscall %u called with args: %u, %u, %u, %u, %u, %u at tick: %u\n", 
        args->syscall_num, args->args[0],args->args[1],args->args[2],args->args[3],args->args[4],args->args[5], system_ticks);
}