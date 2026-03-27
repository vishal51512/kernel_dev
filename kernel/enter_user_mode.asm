[BITS 32]
global enter_user_mode
extern user_start

enter_user_mode:
    cli

    ; DO NOT change DS yet (important)
    mov ax, 0x23        ; USER DATA selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Build minimal IRET frame
    push 0x23           ; SS (user)
    push 0x00E00000     ; ESP (14MB, safe & mapped)
    push 0x202          ; EFLAGS: IF=1 (interrupts enabled), reserved bit 1 set
    push 0x1B           ; CS (user)
    push user_start     ; EIP

    iretd

