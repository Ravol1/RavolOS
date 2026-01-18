#pragma once

#include <stdint.h>
#include "disk.h"

void io_wait(void);

extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, int8_t value);

extern uint16_t inw(uint16_t port);
extern void outw(uint16_t port, int16_t value);

extern uint32_t indw(uint16_t port);
extern void outdw(uint16_t port, int32_t value);

void init_io();