[BITS 32]
global task_switch

task_switch:
    mov eax, [esp + 4]   ; new ESP
    mov ebx, [esp + 8]   ; new EBP
    mov ecx, [esp + 12]  ; new EIP

    mov esp, eax
    mov ebp, ebx
    jmp ecx

