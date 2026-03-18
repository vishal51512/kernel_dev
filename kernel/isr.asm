[BITS 32]
global isr_keyboard
global isr_timer
global isr_page_fault
global isr_gp_fault
extern gp_fault_handler
extern page_fault_handler
extern keyboard_handler
extern timer_handler

isr_page_fault:
    pusha
    call page_fault_handler
    popa
    add esp, 4      ; error code
    iretd
    
isr_timer:
    pusha
    call timer_handler
    popa
    iretd

isr_keyboard:
    pusha
    call keyboard_handler
    popa
    iretd
    

isr_gp_fault:
    pusha
    call gp_fault_handler
    popa
    add esp, 4        ; pop error code pushed by CPU
    iretd
