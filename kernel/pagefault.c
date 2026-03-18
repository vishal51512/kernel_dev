#include "vga.h"
#include <stdint.h>

void page_fault_handler(uint32_t fault_addr) {
  vga_print("\nPAGE FAULT!\nSystem Halted.\n");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

