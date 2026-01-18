#include <stdint.h>
extern volatile uint64_t system_ticks;

void tick_update(void);
void set_pit_frq(uint16_t hz);