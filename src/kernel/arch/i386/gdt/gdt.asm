section .text

global load_tr
global load_gdt
extern _stack_top

load_tr:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]
    
    ltr eax          

    pop ebp
    ret


load_gdt:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]
    lgdt [eax]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.reload_cs


.reload_cs:
    pop ebp
    ret

