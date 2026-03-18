[BITS 16]
[ORG 0x1000]

start:
    mov si, ksmg
    mov ah, 0x0E

.loop:
    lodsb
    cmp al, 0
    je hang
    int 0x10
    jmp .loop
hang:
    jmp hang
ksmg db "kernel loaded into memory!",0

; very important: pad kernel to 512 bytes
times 512-($-$$) db 0

