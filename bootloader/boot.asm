[BITS 16]
[ORG 0x7C00]

KERNEL_OFFSET equ 0x1000
VBE_INFO_ADDR equ 0x3000    ; Temporary safe location for VBE Info

start:
    mov [BOOT_DRIVE], dl

    ; -------------------------
    ; Clear screen (BIOS)
    ; -------------------------
    mov ah, 0x00
    mov al, 0x03
    int 0x10

    mov si, msg_loading
    call print_string

    ; -------------------------
    ; Load kernel
    ; -------------------------
    call load_kernel
    
    ; ============================
    ; Get VBE mode info (REAL MODE)
    ; ============================
    mov ax, 0
    mov es, ax
    mov di, VBE_INFO_ADDR   ; BIOS will write info to 0x3000

    mov ax, 0x4F01          ; Get VBE Mode Info
    mov cx, 0x118           ; Mode 1024x768 (usually)
    int 0x10

    ; --- 1. Read VBE fields from 0x3000 ---
    mov ax, [VBE_INFO_ADDR + 16]   ; pitch
    mov [fb_pitch], ax

    mov ax, [VBE_INFO_ADDR + 18]   ; width
    mov [fb_width], ax

    mov ax, [VBE_INFO_ADDR + 20]   ; height
    mov [fb_height], ax

    mov eax, [VBE_INFO_ADDR + 40]  ; physical address
    mov [fb_phys_addr], eax

    ; --- 2. SET GRAPHICS MODE (Crucial Fix) ---
    mov ax, 0x4F02          ; Set VBE Mode
    mov bx, 0x118
    or bx, 0x4000           ; Enable Linear Framebuffer
    int 0x10

    ; =========================
    ; PHASE 2 — ENTER PROTECTED MODE
    ; =========================
    cli                     
    lgdt [gdt_descriptor]   

    mov eax, cr0
    or eax, 1               
    mov cr0, eax

    jmp 0x08:protected_mode_entry

; =====================================================
; Disk: Load kernel
; =====================================================
load_kernel:
    mov ah, 0x00
    mov dl, [BOOT_DRIVE]
    int 0x13

    mov ah, 0x02
    mov al, 40          ; Read 40 sectors (approx 20KB) just to be safe
    mov ch, 0
    mov cl, 2           
    mov dh, 0
    mov dl, [BOOT_DRIVE]

    mov bx, 0x0000
    mov es, bx
    mov bx, 0x1000
    int 0x13
    jc disk_error
    ret

disk_error:
    mov si, msg_error
    call print_string
    jmp $

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

; =====================================================
; GDT
; =====================================================
gdt_start:
gdt_null:
    dd 0x0
    dd 0x0
gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00
gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; =====================================================
; 32-BIT PROTECTED MODE
; =====================================================
[BITS 32]
global fb_phys_addr
global fb_pitch
global fb_width
global fb_height

protected_mode_entry:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x90000
    jmp 0x08:0x1000 

; =====================================================
; DATA
; =====================================================
[BITS 16]
BOOT_DRIVE db 0
msg_loading db "Loading...", 0
msg_error   db "Disk Err", 0

; Note: We removed 'vbe_mode_info' buffer to save space!

fb_phys_addr dd 0
fb_pitch  dw 0
fb_width  dw 0
fb_height dw 0

; Fill to 510 bytes
times 510-($-$$) db 0
dw 0xAA55
