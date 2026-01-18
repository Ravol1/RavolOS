#include "keyboard.h"
#include "video.h"
#include "kbd_layouts.h"
#include "keyboard_mapping.h"

bool keyboard_map[128];

bool long_code_pending = false;

uint16_t get_keyboard_byte(){
    if (inb(0x64) & 1)
        return inb(0x60);

    return SCANCODE_NONE;
}

uint16_t get_keyboard_code() {
    uint16_t code = get_keyboard_byte();
    
    if(code == 0xE0){
        long_code_pending = true;
        return SCANCODE_NONE;
    }

    if(long_code_pending){
        long_code_pending = false;
        return code + (0xE0 << 4);
    }
    
    return code;
}

void set_keyboard_map(uint16_t scancode){
    if(scancode == SCANCODE_NONE) return;

    printf("Scancode recieved: %x\n", scancode);

    uint8_t base_scancode = (uint8_t)scancode;
    bool is_extended = ((uint8_t)(scancode >> 8) == 0xE0);

    ScancodeMapEntry scancode_info = scancode_map[(uint8_t)scancode];
    uint8_t key = scancode_info.keycode;
    bool pressed = scancode_info.pressed;

    if(is_extended){
        switch (base_scancode)
        {
        case KEY_LCTRL:
            key = is_extended ? KEY_RCTRL : KEY_LCTRL;
            break;
        case KEY_LALT:
            key = is_extended ? KEY_RALT : KEY_LALT;
            break;
        case KEY_LSUPER:
            key = is_extended ? KEY_RSUPER : KEY_LSUPER;
            break;
        
        default:
            break;
        }
    }

    if(keyboard_map[key]){
        if(!pressed){
            keyboard_map[key] = false;
            printf("Released: %x ", key);
        }
    }

    else if(pressed){
        keyboard_map[key] = true;
        printf("Pressed: %x ", key);
    }
}