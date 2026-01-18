[bits 32]
[extern main]
[global _start]

VMEM_BOTTOM equ 0xB8000
VMEM_TOP equ VMEM_BOTTOM+(80*25*2)

_start:
    call clear_screen
    call main
    
    jmp $



clear_screen:
    pusha
    mov eax, VMEM_BOTTOM

cs_loop:
    cmp eax, VMEM_TOP
    jnb done_cs

    mov byte [eax], ' '
    add eax, 2

    jmp cs_loop


done_cs:
    popa
    ret