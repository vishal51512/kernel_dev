[BITS 16]
[ORG 0x7C00]

start:
    ; -------------------------
    ; Set video mode (clear screen)
    ; -------------------------
    mov ah, 0x00
    mov al, 0x03      ; 80x25 text mode
    int 0x10

    ; -------------------------
    ; Print message
    ; -------------------------
    mov si, message
    call print_string

    jmp $

; -------------------------
; Print string routine
; -------------------------
print_string:
    mov ah, 0x0E
.loop:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .loop
.done:
    ret

; -------------------------
; Data
; -------------------------
message db "NovaAI OS Bootloader Started", 0

; -------------------------
; Boot signature
; -------------------------
times 510-($-$$) db 0
dw 0xAA55

