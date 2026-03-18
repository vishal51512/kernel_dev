#include "timer.h"
#include "port.h"
#include "vga.h"
#include "schedular.h"

static unsigned int tick = 0;

void timer_handler() {
   schedular_tick();
   outb(0x20,0x20);  // EOI
}

void timer_init(unsigned int freq) {
    unsigned int divisor = 1193182 / freq;

    outb(0x43, 0x36);           // command byte
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}

