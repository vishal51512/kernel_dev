#include "paging.h"
#include <stdint.h>

#define PAGE_PRESENT  0x1
#define PAGE_RW       0x2

#define NUM_TABLES 4

/* * Page Directory: Holds pointers to Page Tables 
 * Aligned to 4KB (4096 bytes)
 */
static uint32_t page_directory[1024]
__attribute__((aligned(4096)));

/* * Identity Page Tables: Map the first 16MB of RAM 
 */
static uint32_t page_tables[NUM_TABLES][1024]
__attribute__((aligned(4096)));

void paging_init() {

    /* 1. Clear page directory */
    for (int i = 0; i < 1024; i++)
        page_directory[i] = 0;

    /* 2. Identity map first 16MB (Kernel + Code) */
    for (int t = 0; t < NUM_TABLES; t++) {
        for (int i = 0; i < 1024; i++) {
            page_tables[t][i] =
                ((t * 0x400000) + (i * 0x1000)) |
                PAGE_PRESENT | PAGE_RW;
        }

        // Add these tables to the directory
        page_directory[t] =
            ((uint32_t)page_tables[t]) |
            PAGE_PRESENT | PAGE_RW;
    }


    /* 3. Map Framebuffer (at virtual address 0xE0000000) */
    // REMOVED FOR TUI

    /* 4. Load CR3 (Page Directory Base Register) */

    /* 4. Load CR3 (Page Directory Base Register) */
    __asm__ volatile ("mov %0, %%cr3" :: "r"(page_directory));

    /* 5. Enable Paging (Set PG bit in CR0) */
    uint32_t cr0;
    __asm__ volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile ("mov %0, %%cr0" :: "r"(cr0));
}
