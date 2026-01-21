#include "arch/i386/io/io.h"
#include <stdint.h>
#include <stdbool.h>

#define SCANCODE_NONE 0


extern bool keyboard_map[128];


uint16_t get_keyboard_byte(void);
uint16_t get_keyboard_code(void);
void set_keyboard_map(uint16_t scancode);






