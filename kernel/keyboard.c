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
    unsigned int keymap_len = sizeof(keymap) - 1;

    if (scancode & 0x80)
        return; // key release

    if (scancode >= keymap_len) {
        outb(0x20, 0x20); // EOI to PIC
        return;
    }

    char c = keymap[scancode];
    if (c)
        vga_print((char[]){c, 0});

    outb(0x20, 0x20); // EOI to PIC
}
