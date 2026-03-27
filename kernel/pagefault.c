#include "vga.h"
#include <stdint.h>

static void vga_print_hex(uint32_t val) {
    char buf[11];
    const char* hex = "0123456789ABCDEF";
    buf[0] = '0'; buf[1] = 'x';
    for (int i = 0; i < 8; i++)
        buf[2 + i] = hex[(val >> (28 - i * 4)) & 0xF];
    buf[10] = '\0';
    vga_print(buf);
}

void page_fault_handler(uint32_t fault_addr) {
    vga_print("\nPAGE FAULT at address ");
    vga_print_hex(fault_addr);
    vga_print("\nSystem Halted.\n");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

