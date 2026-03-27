#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "task.h"
#include "paging.h"
#include "heap.h"
#include "tss.h"
#include "gdt.h"

extern void enter_user_mode();

void kernel_main() {
    __asm__ volatile ("cli");

    vga_clear();
    vga_print("NovaAI OS\n");

    pic_remap();
    vga_print("PIC remapped\n");

    gdt_init();          // 🔥 REQUIRED
    vga_print("GDT loaded\n");

    tss_init();          // 🔥 MUST come after gdt_init
    vga_print("TSS loaded\n");

    idt_init();
    vga_print("IDT loaded\n");

    timer_init(100);
    vga_print("PIT initialized\n");

    task_init();
    vga_print("Tasks initialized\n");

    paging_init();
    heap_init();

    vga_print("Paging + Heap OK\n");

    __asm__ volatile ("sti"); /* enable interrupts — timer will drive task switching */

    for (;;) {
        __asm__ volatile ("hlt");
    }
}

