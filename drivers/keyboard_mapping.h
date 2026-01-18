#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef enum{
    KEY_NULL  =   0x00,

    KEY_ESCAPE =  0x01,

    KEY_D1 = 0x02,
    KEY_D2 = 0x03,
    KEY_D3 = 0x04,
    KEY_D4 = 0x05,
    KEY_D5 = 0x06,
    KEY_D6 = 0x07,
    KEY_D7 = 0x08,
    KEY_D8 = 0x09,
    KEY_D9 = 0x0A,
    KEY_D0 = 0x0B,
    KEY_MINUS = 0x0C,
    KEY_EQUAL = 0x0D,
    KEY_BACKSPACE = 0x0E,


    KEY_TAB = 0x0F,
    KEY_Q = 0x10,
    KEY_W = 0x11,
    KEY_E = 0x12,
    KEY_R = 0x13,
    KEY_T = 0x14,
    KEY_Y = 0x15,
    KEY_U = 0x16,
    KEY_I = 0x17,
    KEY_O = 0x18,
    KEY_P = 0x19,
    KEY_OSB = 0x1A,     //Open square brackets
    KEY_CSB = 0x1B,     //Close square brackets

    KEY_RETURN = 0x1C,

    KEY_LCTRL = 0x1D,

    KEY_A = 0x1E,
    KEY_S = 0x1F,
    KEY_D = 0x20,
    KEY_F = 0x21,
    KEY_G = 0x22,
    KEY_H = 0x23,
    KEY_J = 0x24,
    KEY_K = 0x25,
    KEY_L = 0x26,
    KEY_SEMICOLON = 0x27,
    KEY_QUOTE = 0x28,

    KEY_BACKTICK = 0x29,

    KEY_LSHIFT = 0x2A,

    KEY_BACKSLASH = 0x2B,


    KEY_Z = 0x2C,
    KEY_X = 0x2D,
    KEY_C = 0x2E,
    KEY_V = 0x2F,
    KEY_B = 0x30,
    KEY_N = 0x31,
    KEY_M = 0x32,
    KEY_COMMA = 0x33,
    KEY_DOT = 0x34,
    KEY_SLASH = 0x35,
    KEY_RSHIFT = 0x36,

    KEY_KP_ASTERISK = 0x37,  
    KEY_LALT = 0x38,
    KEY_SPACEBAR = 0x39,

    KEY_CAPSLOC = 0x3A,

    KEY_F1 = 0x3B,
    KEY_F2 = 0x3C,
    KEY_F3 = 0x3D,
    KEY_F4 = 0x3E,
    KEY_F5 = 0x3F,
    KEY_F6 = 0x40,
    KEY_F7 = 0x41,
    KEY_F8 = 0x42,
    KEY_F9 = 0x43,
    KEY_F10 = 0x44,
    KEY_F11 = 0x57,
    KEY_F12 = 0x58,

    KEY_NUMLOCK = 0x45,
    KEY_SCROLLLOCK = 0x46,
    KEY_HOME = 0x47,
    KEY_UP = 0x48,
    KEY_PGUP = 0x49,
    KEY_KP_MINUS = 0x4A,
    KEY_LEFT = 0x4B,
    KEY_KP_CENTER = 0x4C,
    KEY_RIGHT = 0x4D,
    KEY_KP_PLUS = 0x4E,
    KEY_END = 0x4F,
    KEY_DOWN = 0x50,
    KEY_PGDN = 0x51,
    KEY_INS = 0x52,
    KEY_DEL = 0x53,

    KEY_LSUPER = 0x5B,

    KEY_DEP1 = 0x54,
    KEY_DEP2 = 0x55,
    KEY_DEP3 = 0x56,
    
    KEY_RCTRL = 0x7D,
    KEY_RALT = 0x7E,
    KEY_RSUPER = 0x7F

} Keys;

typedef enum{
    KEY_ESCAPE_UP =  0x81,

    KEY_D1_UP = 0x82,
    KEY_D2_UP = 0x83,
    KEY_D3_UP = 0x84,
    KEY_D4_UP = 0x85,
    KEY_D5_UP = 0x86,
    KEY_D6_UP = 0x87,
    KEY_D7_UP = 0x88,
    KEY_D8_UP = 0x89,
    KEY_D9_UP = 0x8A,
    KEY_D0_UP = 0x8B,
    KEY_MINUS_UP = 0x8C,
    KEY_EQUAL_UP = 0x8D,
    KEY_BACKSPACE_UP = 0x8E,


    KEY_TAB_UP = 0x8F,
    KEY_Q_UP = 0x90,
    KEY_W_UP = 0x91,
    KEY_E_UP = 0x92,
    KEY_R_UP = 0x93,
    KEY_T_UP = 0x94,
    KEY_Y_UP = 0x95,
    KEY_U_UP = 0x96,
    KEY_I_UP = 0x97,
    KEY_O_UP = 0x98,
    KEY_P_UP = 0x99,
    KEY_OSB_UP = 0x9A,     //Open square brackets
    KEY_CSB_UP = 0x9B,     //Close square brackets

    KEY_RETURN_UP = 0x9C,

    KEY_LCTRL_UP = 0x9D,

    KEY_A_UP = 0x9E,
    KEY_S_UP = 0x9F,
    KEY_D_UP = 0xA0,
    KEY_F_UP = 0xA1,
    KEY_G_UP = 0xA2,
    KEY_H_UP = 0xA3,
    KEY_J_UP = 0xA4,
    KEY_K_UP = 0xA5,
    KEY_L_UP = 0xA6,
    KEY_SEMICOLON_UP = 0xA7,
    KEY_QUOTE_UP = 0xA8,

    KEY_BACKTICK_UP = 0xA9,

    KEY_LSHIFT_UP = 0xAA,

    KEY_BACKSLASH_UP = 0xAB,


    KEY_Z_UP = 0xAC,
    KEY_X_UP = 0xAD,
    KEY_C_UP = 0xAE,
    KEY_V_UP = 0xAF,
    KEY_B_UP = 0xB0,
    KEY_N_UP = 0xB1,
    KEY_M_UP = 0xB2,
    KEY_COMMA_UP = 0xB3,
    KEY_DOT_UP = 0xB4,
    KEY_SLASH_UP = 0xB5,
    KEY_RSHIFT_UP = 0xB6,

    KEY_KP_ASTERISK_UP = 0xB7,  
    KEY_LALT_UP = 0xB8,
    KEY_SPACEBAR_UP = 0xB9,

    KEY_CAPSLOC_UP = 0xBA,

    KEY_F1_UP = 0xBB,
    KEY_F2_UP = 0xBC,
    KEY_F3_UP = 0xBD,
    KEY_F4_UP = 0xBE,
    KEY_F5_UP = 0xBF,
    KEY_F6_UP = 0xC0,
    KEY_F7_UP = 0xC1,
    KEY_F8_UP = 0xC2,
    KEY_F9_UP = 0xC3,
    KEY_F10_UP = 0xC4,
    KEY_F11_UP = 0xD7,
    KEY_F12_UP = 0xD8,

    KEY_NUMLOCK_UP = 0xC5,
    KEY_SCROLLLOCK_UP = 0xC6,
    KEY_HOME_UP = 0xC7,
    KEY_UP_UP = 0xC8,
    KEY_PGUP_UP = 0xC9,
    KEY_KP_MINUS_UP = 0xCA,
    KEY_LEFT_UP = 0xCB,
    KEY_KP_CENTER_UP = 0xCC,
    KEY_RIGHT_UP = 0xCD,
    KEY_KP_PLUS_UP = 0xCE,
    KEY_END_UP = 0xCF,
    KEY_DOWN_UP = 0xD0,
    KEY_PGDN_UP = 0xD1,
    KEY_INS_UP = 0xD2,
    KEY_DEL_UP = 0xD3,

    KEY_LSUPER_UP = 0xDB,

    KEY_DEP1_UP = 0xD4,
    KEY_DEP2_UP = 0xD5,
    KEY_DEP3_UP = 0xD6

} Keys_up;

typedef struct {
    uint8_t keycode;  // enum Keys
    bool pressed;     // true = press, false = release
} ScancodeMapEntry;

extern ScancodeMapEntry scancode_map[256];