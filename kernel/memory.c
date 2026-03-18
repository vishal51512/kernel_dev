#include "memory.h"

void *memset(void *ptr, int value, uint32_t size) {
    unsigned char *p = (unsigned char *)ptr;
    while (size--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}

