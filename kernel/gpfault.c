#include "vga.h"

void gp_fault_handler() {
    vga_print("\n#GP FAULT (General Protection Fault)\n");
    vga_print("System halted.\n");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

