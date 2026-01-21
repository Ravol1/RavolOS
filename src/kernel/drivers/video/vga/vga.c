#include <stdarg.h>
#include <ltostr.h>
#include "vga.h"
#include "arch/i386/io/io.h"

uint16_t cursor_position = 0;
uint8_t video_mode = LIGHTGREEN_BLAK;
volatile uint16_t* video_memory = (volatile uint16_t*)VIDEO_MEMORY_START;


void vga_scroll(){
    uint16_t last_row = SCREEN_SIZE - SCREEN_LENGTH;
    for(uint16_t i = 0; i < last_row; ++i){
        video_memory[i] = video_memory[i + SCREEN_LENGTH];
    }

    for (size_t i = last_row; i < SCREEN_SIZE; ++i)
    {
        video_memory[i] = ' ' | ((uint16_t) video_mode << 8);
    }
    
    cursor_position -= SCREEN_LENGTH;
}


void vga_move_cursor(uint32_t offset){
    outb(CRT_INDEX_REG, CURSOR_LOW_BYTE);
    outb(CRT_DATA_REG, offset & 0x00FF);
    outb(CRT_INDEX_REG, CURSOR_HIGH_BYTE);
    outb(CRT_DATA_REG, (offset >> 8) & 0xFF);
}

void vga_new_line(){
    uint16_t line = cursor_position/SCREEN_LENGTH +1;
    cursor_position = line * SCREEN_LENGTH;

    if (cursor_position >= SCREEN_SIZE){
        vga_scroll();
    }
}


void vga_printch(char character, uint16_t position, bool update_cursor){

    if(character == '\n' && update_cursor){
        vga_new_line();
        vga_move_cursor(cursor_position);
        return;
    }

    else if(character == 0){
        return;
    }

    video_memory[position] = character | ((uint16_t)video_mode << 8);

    if(update_cursor){
        ++cursor_position;
        vga_move_cursor(cursor_position);
    }
}


void vga_putchar(char cahracter){
    if (cursor_position >= SCREEN_SIZE){
        vga_scroll();
    }

    vga_printch(cahracter, cursor_position, true);
}


void vga_print_decimal(long int number){
    char str[66];
    ltostr(number, 10, str, 66);

    for(const char* i = str; *i != 0; ++i){
        vga_putchar(*i);
    }
}


void vga_print_hex(long int number){
    char str[66];
    ltostr(number, 16, str, 66);
    vga_putchar('0'); vga_putchar('x');

    for(const char* i = str; *i != 0; ++i){
        vga_putchar(*i);
    }
}


void vga_print(const char* fmt, ...){
    va_list args;
    va_start(args, fmt);

    for (const char* p = fmt; *p != '\0'; p++) {
        if (*p != '%') {
            vga_putchar(*p);
            continue;
        }

        p++; // Move past '%'

        switch (*p) {
            case 's': {
                const char* str = va_arg(args, const char*);

                for(const char* i = str; *i != 0; ++i){
                    vga_putchar(*i);
                }

                break;
            }
            case 'd': {
                int val = va_arg(args, int);
                vga_print_decimal(val);
                break;
            }
            case 'i': {
                int val = va_arg(args, int);
                vga_print_decimal(val);
                break;
            }
            case 'u': {
                int val = va_arg(args, int);
                vga_print_decimal(val);
                break;
            }
            case 'x': {
                unsigned int val = va_arg(args, unsigned int);
                vga_print_hex(val);
                break;
            }
            case 'c': {
                char c = (char) va_arg(args, int); // `char` is promoted to `int` in varargs
                vga_putchar(c);
                break;
            }
            case 'p':{
                unsigned int val = va_arg(args, unsigned int);
                char str[66];
                ltostr(val, 16, str, 66);

                for(const char* i = str; *i != 0; ++i){
                    vga_putchar(*i);
                }

                break;
            }
            case '%': {
                vga_putchar('%'); // Escaped percent "%%"
                break;
            }
            default: {
                // Unknown specifier, just print it
                vga_putchar('%');
                vga_putchar(*p);
                break;
            }
        }
    }

    va_end(args);
}


void vga_erase(uint16_t pos, bool update_cursor){
    --pos;
    vga_printch(' ', pos, false);

    if(update_cursor){
        cursor_position = pos;
        vga_move_cursor(cursor_position);
    }
}

void vga_clear_screen(){
    for(uint16_t i = 0; i<SCREEN_SIZE; ++i){
        video_memory[i] = ' ' | ((uint16_t)video_mode << 8); 
    }
}

void vga_init(){
    video_mode = LIGHTGREEN_BLAK;
    cursor_position = 0;
    vga_move_cursor(cursor_position);
    vga_clear_screen();
}
