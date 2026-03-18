#include "tss.h"
#include "gdt.h"
#include "memory.h"

static struct tss_entry tss;

void tss_init() {
    memset(&tss, 0, sizeof(tss));

    tss.ss0  = 0x10;      // kernel data
    tss.esp0 = 0x90000;   // kernel stack

    uint32_t base  = (uint32_t)&tss;
    uint32_t limit = sizeof(tss);

    gdt_set_gate(5, base, limit, 0x89, 0x00); // TSS descriptor
    __asm__ volatile ("ltr %%ax" :: "a"(0x28));
}

