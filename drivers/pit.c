#include <stdbool.h>
#include <stddef.h>
#include "pit.h"
#include "io.h"

#define PIT_BASE_FREQ 1193182


#define PIT_CHANNEL0    0x40
#define PIT_CHANNEL1    0x41
#define PIT_CHANNEL2    0x42
#define PIT_COMMAND     0x43

#define PIT_RB_CH0      0x02
#define PIT_RB_CH1      0x04
#define PIT_RB_CH2      0x08
#define PIT_RB          0xC0
#define PIT_RB_COUNT    0x10
#define PIT_RB_STATUS   0x20

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
} pit_access;


bool pit_set_count(unsigned int channel, uint16_t count, pit_mode mode){
    if(channel > 2) return NULL;
    int ch = PIT_CHANNEL0 + channel;

    outb(PIT_COMMAND, PIT_CH_SEL_CH0 | PIT_ACC_LH | mode);


    outb(ch, count & 0xFF);
    outb(ch, count >> 8);
    
    return true;
}

bool pit_set_frequency(uint8_t channel, uint32_t hz, pit_mode mode) {
    if(hz == 0 || hz >= PIT_BASE_FREQ) return NULL;
    uint16_t count = PIT_BASE_FREQ / hz;
    return pit_set_count(channel, count, mode);
}

pit_status_t pit_readback_status(uint8_t channel){
    pit_status_t status;
    if(channel > 2) return status;

    uint8_t ch = PIT_CHANNEL0 + channel;

    outb(PIT_COMMAND, PIT_RB | PIT_RB_STATUS | PIT_RB_CH1);

    uint8_t raw = inb(ch);
    
    status.out_pin = (raw & 0b10000000) >> 7;       // bit 7 
    status.null_count = (raw & 0b01000000) >> 6;    // bit 6
    status.mode = raw & 0b00111111;                 // bits 5-0

    return status;
}
