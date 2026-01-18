[bits 32]
[extern main]
[global _start]


section .multiboot
align 4
    dd 0x1BADB002              ; magic number
    dd 0x00000000              ; flags
    dd -(0x1BADB002 + 0x00000000)  ; checksum

section .text
_start:
    cli
    
    call clear_screen
    call main
    jmp $


VMEM_BOTTOM equ 0xB8000
VMEM_TOP equ VMEM_BOTTOM+(80*25*2)

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