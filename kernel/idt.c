#include "idt.h"

extern void isr_timer();
extern void isr_keyboard();
extern void isr_page_fault();
extern void isr_gp_fault();
extern void isr_syscall();
static struct idt_entry idt[256];
static struct idt_ptr idtp;

void idt_set_gate(int n, uint32_t handler, uint8_t flags) {
    idt[n].base_low  = handler & 0xFFFF;
    idt[n].base_high = (handler >> 16) & 0xFFFF;
    idt[n].sel = 0x08;        // kernel code selector
    idt[n].always0 = 0;
    idt[n].flags = flags;
}

void idt_init() {
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)&idt;

    // Clear IDT safely
    for (int i = 0; i < 256; i++) {
        idt[i].base_low  = 0;
        idt[i].base_high = 0;
        idt[i].sel       = 0;
        idt[i].always0  = 0;
        idt[i].flags    = 0;
    }

    // Hardware IRQs (Ring 0 only)
    idt_set_gate(32, (uint32_t)isr_timer,    0x8E);
    idt_set_gate(33, (uint32_t)isr_keyboard, 0x8E);

    // CPU exceptions (must be callable from Ring 3)
    idt_set_gate(13, (uint32_t)isr_gp_fault,   0xEE); // #GP
    idt_set_gate(14, (uint32_t)isr_page_fault, 0xEE); // #PF
    

    //syscall
    idt_set_gate(0x80, (uint32_t)isr_syscall, 0xEE);

    __asm__ volatile ("lidt %0" : : "m"(idtp));
}

