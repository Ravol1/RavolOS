#include <stdint.h>
#include <stdbool.h>

#define TICK_PER_SECOND 1000

extern volatile uint64_t system_ticks;

void tick_update(void);
bool set_system_clock(uint16_t hz);
void kernel_wait(uint32_t ns);