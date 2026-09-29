#include "video.h"

#include <stdarg.h>
#include <ltostr.h>

#include "drivers/video/vga/vga.h"



typedef void (*putc_fn_t)(char);

putc_fn_t get_console_sink(){
    return vga_putchar;
}




void kprint_decimal(void (*putc)(char), long int number){
    char str[66];
    ltostr(number, 10, str, 66);

    for(const char* i = str; *i != 0; ++i){
        putc(*i);
    }
}


void kprint_hex(void (*putc)(char), long int number){
    char str[66];
    ltostr(number, 16, str, 66);
    putc('0'); putc('x');

    for(const char* i = str; *i != 0; ++i){
        putc(*i);
    }
}



void kvprintf_to(void (*putc)(char), const char* fmt, va_list args){
    for (const char* p = fmt; *p != '\0'; p++) {
        if (*p != '%') {
            putc(*p);
            continue;
        }

        p++; // Move past '%'

        switch (*p) {
            case 's': {
                const char* str = va_arg(args, const char*);

                for(const char* i = str; *i != 0; ++i){
                    putc(*i);
                }

                break;
            }
            case 'd':
            case 'i': {
                int val = va_arg(args, int);
                kprint_decimal(putc, val);
                break;
            }
            
            case 'x': {
                unsigned int val = va_arg(args, unsigned int);
                kprint_decimal(putc, val);
                break;
            }
            case 'c': {
                char c = (char) va_arg(args, int); // `char` is promoted to `int` in varargs
                putc(c);
                break;
            }
            case 'p':{
                unsigned int val = va_arg(args, unsigned int);
                char str[66];
                ltostr(val, 16, str, 66);

                for(const char* i = str; *i != 0; ++i){
                    putc(*i);
                }

                break;
            }
            case '%': {
                putc('%'); // Escaped percent "%%"
                break;
            }
            default: {
                // Unknown specifier, just print it
                putc('%');
                putc(*p);
                break;
            }
        }
    }
}


void kprintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    kvprintf_to(get_console_sink(), fmt, args);
    va_end(args);
}


void kputchar(char character){
    putc_fn_t putc = get_console_sink();
    putc(character);
}



void clear_screen(){
    vga_clear_screen();
}


void video_init(){
    vga_init();
}