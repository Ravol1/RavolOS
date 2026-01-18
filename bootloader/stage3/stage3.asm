[bits 32]

global kernel_entry
KERNEL_LOCATION equ 0x100000
KER_DATA_SEG equ 0x10


kernel_entry:
    ; Reset stack

    mov ax, KER_DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    mov ebp, 0x200000
    mov esp, ebp

    jmp KERNEL_LOCATION