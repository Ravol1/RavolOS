#pragma once

#include <stdint.h>
#include <stdbool.h>

#define VIDEO_MEMORY_START (0xB8000)
#define SCREEN_HEIGHT 25
#define SCREEN_LENGTH 80
#define SCREEN_SIZE (SCREEN_HEIGHT*SCREEN_LENGTH)
#define VIDEO_MEMORY_SIZE (2*SCREEN_SIZE)
#define VIDEO_MEMORY_LIMIT (VIDEO_MEMORY_START + VIDEO_MEMORY_SIZE)

#define CRT_INDEX_REG 0x3D4
#define CRT_DATA_REG  0x3D5
#define CURSOR_LOW_BYTE 0x0F
#define CURSOR_HIGH_BYTE 0x0E



extern uint16_t cursor_position;
extern uint8_t video_mode;

enum Video_Modes {
    WHITE_BLACK     =   0x0F,
    LIGHTGRAY_BLAK  =   0x07,
    WHITE_BLUE      =   0x1F,
    LIGHTGRAY_BLUE  =   0x17,
    GREEN_BLACK     =   0x02,
    LIGHTGREEN_BLAK =   0x0A

};  

void vga_putchar(char cahracter);
void vga_print(const char* fmt, ...);
void vga_clear_screen();
void vga_erase(uint16_t pos, bool update_cursor);

void vga_init();