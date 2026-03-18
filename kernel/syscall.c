#include "vga.h"
#include <stdint.h>

void syscall_handler(uint32_t eax, uint32_t ebx) {
    switch (eax) {
        case 0:  // sys_write
            vga_putchar((char)ebx);
            break;
        default:
            break;
    }
}

