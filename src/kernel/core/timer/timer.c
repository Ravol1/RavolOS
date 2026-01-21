#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "drivers/x86/i386/pit/pit.h"

#define WAIT_COUNT (1193182/10000)      // 10'000 Hz



volatile uint64_t system_ticks = 0;

void tick_update(){
    system_ticks++;
}

bool set_system_clock(uint16_t hz){
    uint16_t count = 1193182 / hz;

    pit_set_count(0, count, PIT_MODE_M3);
}

void kernel_wait_ms(uint32_t ms){ 
    uint64_t target = system_ticks + ms;

    while (system_ticks < target){
        asm volatile("hlt");
    }
}


