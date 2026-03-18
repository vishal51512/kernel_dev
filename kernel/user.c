#include <stdint.h>
static inline void syscall_write(char c) {
    __asm__ volatile (
        "mov $0, %%eax\n"     // syscall number
        "mov %0, %%ebx\n"     // argument
        "int $0x80"
        :
        : "r"((uint32_t)c)    // 🔥 force 32-bit
        : "eax", "ebx"
    );
}

void user_start() {
    while (1) {
        syscall_write('U');
        for (volatile int i = 0; i < 1000000; i++);
    }
}

