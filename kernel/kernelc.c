#include "vga.h"

void task1_entry() {
    while (1) {
        vga_print("A");
        for (volatile int i = 0; i < 1000000; i++);
    }
}

void task2_entry() {
    while (1) {
        vga_print("B");
        for (volatile int i = 0; i < 1000000; i++);
    }
}

