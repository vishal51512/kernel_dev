#include "heap.h"
#include "vga.h"

#define HEAP_START 0x01000000   // 16 MB
#define HEAP_SIZE  0x00100000   // 1 MB

static uint32_t heap_curr;

void heap_init() {
    heap_curr = HEAP_START;
//    vga_print("Kernel heap initialized\n");
}

void* kmalloc(size_t size) {
    if (size == 0)
        return 0;

    // Align to 8 bytes
    size = (size + 7) & ~7;

    void* addr = (void*)heap_curr;
    heap_curr += size;

    if (heap_curr >= HEAP_START + HEAP_SIZE) {
        vga_print("Heap overflow!\n");
        return 0;
    }

    return addr;
}

void kfree(void* ptr) {
    // No-op for bump allocator (yet)
    (void)ptr;
}

