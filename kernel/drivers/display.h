#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdint.h>

// Hard-coded display values
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

enum BASE_DISPLAY_COLOR
{
    BASE_DISPLAY_COLOR_BLACK         = 0,
    BASE_DISPLAY_COLOR_BLUE          = 1,
    BASE_DISPLAY_COLOR_GREEN         = 2,
    BASE_DISPLAY_COLOR_CYAN          = 3,
    BASE_DISPLAY_COLOR_RED           = 4,
    BASE_DISPLAY_COLOR_MAGENTA       = 5,
    BASE_DISPLAY_COLOR_BROWN         = 6,
    BASE_DISPLAY_COLOR_LIGHTGRAY     = 7,
    BASE_DISPLAY_COLOR_DARKGRAY      = 8,
    BASE_DISPLAY_COLOR_LIGHTBLUE     = 9,
    BASE_DISPLAY_COLOR_LIGHTGREEN    = 10,
    BASE_DISPLAY_COLOR_LIGHTCYAN     = 11,
    BASE_DISPLAY_COLOR_LIGHTRED      = 12,
    BASE_DISPLAY_COLOR_PINK          = 13,
    BASE_DISPLAY_COLOR_YELLOW        = 14,
    BASE_DISPLAY_COLOR_WHITE         = 15
};

// Generates a foreground/background color combo
static inline uint8_t make_display_color(uint8_t foreground, uint8_t background) 
{
    return (background << 4) | foreground;
} 

void print(char *message, uint8_t color);

/* 
 * Prints an ASCII character at a screen position
 * Leave char_color at 0 to do default black on white
 */
void print_char(uint8_t character, uint8_t col, uint8_t row, uint8_t char_color);

/*
 * Clears the screen
 */
void clear_screen();

#endif