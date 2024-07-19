#include "screen.h"
#include "io.h"

#define VIDEO_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* const VGA_MEMORY = (uint16_t*)VIDEO_MEMORY;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t)uc | (uint16_t)color << 8;
}

void move_cursor() {
    uint16_t cursor_location = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 14);
    outb(0x3D5, cursor_location >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cursor_location);
}

void screen_clear() {
    uint16_t blank = vga_entry(' ', 0x07);
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            VGA_MEMORY[y * VGA_WIDTH + x] = blank;
        }
    }
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void screen_print_char(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(c, 0x07);
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }

    if (cursor_y >= VGA_HEIGHT) {
        cursor_y = 0;
    }

    move_cursor();
}

void screen_print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        screen_print_char(str[i]);
    }
}
