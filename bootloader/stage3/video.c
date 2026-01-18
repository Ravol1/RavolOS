#include <stdarg.h>
#include <stddef.h>
#include "video.h"
#include "io.h"

uint16_t cursor_position = 0;
uint8_t video_mode = LIGHTGRAY_BLAK;

static void invstr(const char* start, const char* end){
    for (char *start_cpy = (char*)start, *end_cpy = (char*)end; start_cpy < end_cpy; ++start_cpy, --end_cpy){
        char tmp = *start_cpy;
        *start_cpy = *end_cpy; 
        *end_cpy = tmp;
    }
}

static char* ltostr(long int num, int base){
    if(base < 0 || base > 36) return NULL;

    static char out_buf[12];
    char* ptr = out_buf;   

    char* start;


    if(base <= 10){
        if(base == 2){
            *ptr = '0';
            ++ptr;
            *ptr = 'b';
            ++ptr;
        }

        start = ptr;

        do{
            char remainder = num%base;
            num = num/base;

            (*ptr) = remainder | 0b00110000;    // Converts numerical value to corresponding ascii character
            ++ptr;
        }while(num != 0);

    }

    else{
        if(base == 16){
            *ptr = '0';
            ++ptr;
            *ptr = 'x';
            ++ptr;
        } 

        start = ptr;

        do{
            char remainder = num%base;
            num = num / base;

            remainder = remainder | 0b00110000;     // Converts numerical value to corresponding ascii character
            if(remainder > '9'){
                remainder += ('A' - '9' - 1);
            }
            
            (*ptr) = remainder;
            ++ptr;
        } while(num != 0);
    }

    (*ptr) = '\0';
    char* end = --ptr;
    invstr(start, end);
    
    return out_buf;
}



void move_cursor(uint32_t offset){
    outb(CRT_INDEX_REG, CURSOR_LOW_BYTE);
    outb(CRT_DATA_REG, offset & 0x00FF);
    outb(CRT_INDEX_REG, CURSOR_HIGH_BYTE);
    outb(CRT_DATA_REG, (offset >> 8) & 0xFF);
}

void new_line(){
    uint16_t line = cursor_position/SCREEN_LENGTH +1;

    cursor_position = line * SCREEN_LENGTH;
}


void printch(char character, uint16_t position, bool update_cursor){

    if(character == '\n' && update_cursor){
        new_line();
        move_cursor(cursor_position);
        return;
    }

    else if(character == 0){
        return;
    }

    uint32_t addr = VIDEO_MEMORY_START + position*2;
    *(volatile int8_t*)addr = character;
    *(volatile int8_t*)(addr+1) = video_mode;

    if(update_cursor){
        ++cursor_position;
        move_cursor(cursor_position);
    }
}


void putchar(char cahracter){
    printch(cahracter, cursor_position, true);
}


void print_string(const char* in_str, uint16_t start_location, bool update_cursor){
    if (update_cursor){
        cursor_position = start_location;
        
        for(uint16_t i = 0; in_str[i] != 0 && cursor_position <= SCREEN_SIZE; i++)
        {
            char ch = in_str[i];
            int8_t* addr = (int8_t*)(2*cursor_position + VIDEO_MEMORY_START);

            if(ch == '\n'){
                new_line();
                continue;
            }

            *addr = ch;
            *(addr+1) = video_mode;

            cursor_position++;
        }

        move_cursor(cursor_position);
    }

    else{
        int8_t* addr = (int8_t*)(2*start_location + VIDEO_MEMORY_START);

        for(size_t i = 0; in_str[i] != 0; i++)
        {
            char ch = in_str[i];    
            if(ch == '\n'){
                uint16_t line = ((size_t)addr - VIDEO_MEMORY_START) / 2/SCREEN_HEIGHT + 1;
                addr = (int8_t*)(VIDEO_MEMORY_START + line*SCREEN_LENGTH*2);
                continue;
            }

            *addr = ch;
            *(addr+1) = video_mode;

            addr+=2;
        }
    }
    
}


void print_decimal(size_t number, uint16_t position, bool update_cursor){
    print_string(ltostr(number, 10), position, update_cursor);
}


void print_hex(size_t number, uint16_t position, bool update_cursor){
    print_string(ltostr(number, 16), position, update_cursor);
}


void printf(const char* fmt, ...){
    va_list args;
    va_start(args, fmt);

    for (const char* p = fmt; *p != '\0'; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }

        p++; // Move past '%'

        switch (*p) {
            case 's': {
                const char* str = va_arg(args, const char*);
                print_string(str, cursor_position, true);
                break;
            }
            case 'd': {
                int val = va_arg(args, int);
                print_decimal(val, cursor_position, true);
                break;
            }
            case 'i': {
                int val = va_arg(args, int);
                print_decimal(val, cursor_position, true);
                break;
            }
            case 'x': {
                unsigned int val = va_arg(args, unsigned int);
                print_hex(val, cursor_position, true);
                break;
            }
            case 'c': {
                char c = (char) va_arg(args, int); // `char` is promoted to `int` in varargs
                putchar(c);
                break;
            }
            case '%': {
                putchar('%'); // Escaped percent "%%"
                break;
            }
            default: {
                // Unknown specifier, just print it
                putchar('%');
                putchar(*p);
                break;
            }
        }
    }

    va_end(args);
}


void clear_screen(){
    volatile int8_t* vidmem = (volatile int8_t*) VIDEO_MEMORY_START;
    for(uint16_t i = 0; i<2*SCREEN_SIZE; i += 2){
        vidmem[i] = ' ';
        vidmem[i+1] = video_mode;
    }
}


void init_video(){
    video_mode = LIGHTGRAY_BLAK;
    cursor_position = 0;
    move_cursor(cursor_position);
}

