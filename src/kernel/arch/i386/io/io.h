#ifndef IO_H
#define IO_H

#include <stdint.h>

extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, int8_t value);

extern uint16_t inw(uint16_t port);
extern void outw(uint16_t port, int16_t value);

extern uint32_t indw(uint16_t port);
extern void outdw(uint16_t port, int32_t value);

static inline void io_wait(void){
    outb(0x80, 0);
}

void init_io();

#endif