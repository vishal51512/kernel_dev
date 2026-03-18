#ifndef TSS_H
#define TSS_H

#include <stdint.h>

struct tss_entry {
    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t unused[23];
} __attribute__((packed));

void tss_init();

#endif

