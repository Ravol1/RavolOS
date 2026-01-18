
#include "keyboard_mapping.h"

typedef struct {
    char base;          
    char shift;        
    char alt_gr;        
    char shift_alt_gr;   
} KeyMapping;

static KeyMapping qwerty_us_layout[128] = {
    [KEY_A] = {'a', 'A', 0, 0},
    [KEY_B] = {'b', 'B', 0, 0},
    [KEY_C] = {'c', 'C', 0, 0},
    [KEY_D] = {'d', 'D', 0, 0},
    [KEY_E] = {'e', 'E', 0, 0},
    [KEY_F] = {'f', 'F', 0, 0},
    [KEY_G] = {'g', 'G', 0, 0},
    [KEY_H] = {'h', 'H', 0, 0},
    [KEY_I] = {'i', 'I', 0, 0},
    [KEY_J] = {'j', 'J', 0, 0},
    [KEY_K] = {'k', 'K', 0, 0},
    [KEY_L] = {'l', 'L', 0, 0},
    [KEY_M] = {'m', 'M', 0, 0},
    [KEY_N] = {'n', 'N', 0, 0},
    [KEY_O] = {'o', 'O', 0, 0},
    [KEY_P] = {'p', 'P', 0, 0},
    [KEY_Q] = {'q', 'Q', 0, 0},
    [KEY_R] = {'r', 'R', 0, 0},
    [KEY_S] = {'s', 'S', 0, 0},
    [KEY_T] = {'t', 'T', 0, 0},
    [KEY_U] = {'u', 'U', 0, 0},
    [KEY_V] = {'v', 'V', 0, 0},
    [KEY_W] = {'w', 'W', 0, 0},
    [KEY_X] = {'x', 'X', 0, 0},
    [KEY_Y] = {'y', 'Y', 0, 0},
    [KEY_Z] = {'z', 'Z', 0, 0},

    [KEY_D1] = {'1', '!', 0, 0},
    [KEY_D2] = {'2', '@', 0, 0},
    [KEY_D3] = {'3', '#', 0, 0},
    [KEY_D4] = {'4', '$', 0, 0},
    [KEY_D5] = {'5', '%', 0, 0},
    [KEY_D6] = {'6', '^', 0, 0},
    [KEY_D7] = {'7', '&', 0, 0},
    [KEY_D8] = {'8', '*', 0, 0},
    [KEY_D9] = {'9', '(', 0, 0},
    [KEY_D0] = {'0', ')', 0, 0},

    [KEY_SPACEBAR] = {' ', ' ', ' ',' '},
    [KEY_MINUS] = {'-', '_', 0, 0},
    [KEY_EQUAL] = {'=', '+', 0, 0},
    [KEY_OSB] = {'[', '{', 0, 0},
    [KEY_CSB] = {']', '}', 0, 0},
    [KEY_SEMICOLON] = {';', ':', 0, 0},
    [KEY_QUOTE] = {'\'', '"', 0, 0},
    [KEY_BACKTICK] = {'`', '~', 0, 0},
    [KEY_BACKSLASH] = {'\\', '|', 0, 0},
    [KEY_COMMA] = {',', '<', 0, 0},
    [KEY_DOT] = {'.', '>', 0, 0},
    [KEY_SLASH] = {'/', '?', 0, 0},
    [KEY_KP_ASTERISK] = {'*', '*', '*','*'}

};
