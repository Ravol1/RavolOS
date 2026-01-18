#include <stdint.h>
#include "timer.h"
#include "../drivers/io.h"


#define PIT_CHANNEL0    0x40
#define PIT_CHANNEL1    0x41
#define PIT_CHANNEL2    0x42
#define PIT_COMMAND     0x43


typedef enum{
    PIT_CH_SEL_CH0 = 0x00,
    PIT_CH_SEL_CH1 = 0x40,
    PIT_CH_SEL_CH2 = 0x80,
    PIT_CH_SEL_RB = 0xC0
} pit_channel_sel;

typedef enum{
    PIT_ACC_LCVC = 0x00,
    PIT_ACC_L = 0x10,
    PIT_ACC_H = 0x20,
    PIT_ACC_LH = 0x30
} pit_access_mode;

typedef enum{
    PIT_OP_MODE_M0 = 0x00,      // interrupt on terminal count
    PIT_OP_MODE_M1 = 0x02,      // hardware re-triggerable one-shot
    PIT_OP_MODE_M2 = 0x04,      // rate generator
    PIT_OP_MODE_M3 = 0x06,      // square wave generator
    PIT_OP_MODE_M4 = 0x08,      // software triggered strobe
    PIT_OP_MODE_M5 = 0x0A       // hardware triggered strobe
} pit_operating_mode;

typedef enum{
    PIT_MODE_BIN = 0x00,
    PIT_MODE_BCD = 0x01
} pit_mode;





volatile uint64_t system_ticks = 0;

void tick_update(){
    system_ticks++;
}


void set_pit_frq(uint16_t hz){
    uint16_t count = 1193182 / hz;

    outb(PIT_COMMAND, PIT_CH_SEL_CH0 | PIT_ACC_LH | PIT_OP_MODE_M3);


    outb(PIT_CHANNEL0, count & 0xFF);
    outb(PIT_CHANNEL0, count >> 8);
}