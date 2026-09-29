#include <stdarg.h>
#include <ltostr.h>
#include "vga.h"
#include "drivers/io/io.h"

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
    cursor_position = 0;
}

void vga_init(){
    video_mode = LIGHTGREEN_BLAK;
    cursor_position = 0;
    vga_move_cursor(cursor_position);
    vga_clear_screen();
}
