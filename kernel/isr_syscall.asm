[BITS 32]
global isr_syscall
extern syscall_handler

isr_syscall:
    pusha
    push ebx
    push eax
    call syscall_handler
    add esp, 8
    popa
    iretd

