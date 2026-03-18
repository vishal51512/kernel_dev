#include "keyboard.h"
#include "port.h"
#include "vga.h"

static char keymap[] =
"??1234567890-=\b"
"\tqwertyuiop[]\n"
"?asdfghjkl;'`"
"?\\zxcvbnm,./?"
"*? ";

void keyboard_handler() {
    unsigned char scancode = inb(0x60);

    if (scancode & 0x80)
        return; // key release

    char c = keymap[scancode];
    if (c)
        vga_print((char[]){c, 0});

    outb(0x20, 0x20); // EOI to PIC
}

