#include "vga.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static unsigned short* vga = (unsigned short*) VGA_ADDRESS;
static int cursor = 0;
static unsigned char color = 0x0F; // white on black
static unsigned short* vga_buffer = (unsigned short*)0xB8000;
static int cursor_x = 0;
static int cursor_y = 0;

static unsigned short vga_entry(char c) {
    return (unsigned short) c | (unsigned short) color << 8;
}

void vga_clear() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga[i] = vga_entry(' ');
    cursor = 0;
}

void vga_print(const char* str) {
    for (int i = 0; str[i] != 0; i++) {
        if (str[i] == '\n')
            cursor += VGA_WIDTH - (cursor % VGA_WIDTH);
        else
            vga[cursor++] = vga_entry(str[i]);
    }
}

void vga_putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        return;
    }

    vga_buffer[cursor_y * 80 + cursor_x] =
        (color << 8) | c;

    cursor_x++;

    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }
}
