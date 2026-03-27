#include "vga.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH   80
#define VGA_HEIGHT  25

static unsigned short* vga = (unsigned short*)VGA_ADDRESS;
static unsigned char color = 0x0F; /* white on black */
static int cursor_x = 0;
static int cursor_y = 0;

static unsigned short vga_entry(char c) {
    return (unsigned short)c | (unsigned short)color << 8;
}

/* Scroll the screen up one line. */
static void vga_scroll() {
    /* Move every row up by one. */
    for (int row = 1; row < VGA_HEIGHT; row++)
        for (int col = 0; col < VGA_WIDTH; col++)
            vga[(row - 1) * VGA_WIDTH + col] = vga[row * VGA_WIDTH + col];

    /* Clear the last row. */
    for (int col = 0; col < VGA_WIDTH; col++)
        vga[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = vga_entry(' ');

    cursor_y = VGA_HEIGHT - 1;
}

void vga_clear() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga[i] = vga_entry(' ');
    cursor_x = 0;
    cursor_y = 0;
}

void vga_putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        vga[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(c);
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }

    if (cursor_y >= VGA_HEIGHT)
        vga_scroll();
}

void vga_print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++)
        vga_putchar(str[i]);
}
