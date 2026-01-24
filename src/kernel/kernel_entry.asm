[bits 32]
[extern main]
[global _start]

extern _stack_top
extern gdt_init

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
    mov [multiboot_info], ebx
    mov [multiboot_magic], eax

    call gdt_init
    
    mov esp, _stack_top
    mov ebp, esp

    mov eax, [multiboot_info]
    push eax
    mov eax, [multiboot_magic]
    push eax
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


section .data
multiboot_info:
    dd 0
multiboot_magic:
    dd 0