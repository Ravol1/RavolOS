[bits 32]
[extern main]
[global _start]

section .multiboot
align 8
header_start:
    dd 0xE85250D6                   ; magic
    dd 0                            ; architecture (0 = i386)
    dd header_start - header_end    ; total_length (header + checksum = 16 byte)
    dd -(0xE85250D6 + 0 + (header_start - header_end))       ; checksum

    align 8
    dd 0    ; type (0 = end tag)
    dd 0    ; flags
    dd 8    ; size
header_end:

section .text
_start:
    cli
    
    ;call clear_screen
    
    push ebx        ; Pointer to multiboot info
    push eax        ; Magic number
    call main
    add esp, 8

    halt:
    hlt
    jmp halt

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