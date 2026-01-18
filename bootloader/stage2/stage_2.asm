[org 0x7e00]
[bits 16]

VMEM_BOTTOM equ 0xB8000
VMEM_TOP equ VMEM_BOTTOM+(80*25*2)

_stage_2_start:
    mov [HARD_DISK], dh
    mov [BOOT_DISK], dl

    call load_stage_3

    cli                     ;Disables interrupts
    lgdt [GDT_Descriptor]   ;Loads Global Descriptor Table

    mov eax, cr0
    or eax, 00000001b 
    mov cr0, eax            ;yay, 32 bit mode protected mode!!

    jmp KER_CODE_SEG:start_protected_mode


[bits 32]
start_protected_mode:
    call clear_screen

    mov ax, KER_DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp

    jmp STAGE_3_LOC
    
end:
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


%include "bootloader/stage2/print_string.asm"
%include "bootloader/stage2/invert_string.asm"
%include "bootloader/stage2/new_line.asm"
%include "bootloader/stage2/load_stage_3.asm"
%include "bootloader/stage2/num_to_string.asm"
%include "bootloader/stage2/num_to_string_hex.asm"
%include "bootloader/stage2/GDT.asm"

disk_packet:
    db 0x10             ; Size of packet
    db 0                ; Reserved
sectors_to_load:
    dw 0
loading_offset:
    dw 0                ; Memory ofset
loading_segment:
    dw 0                ; Memory segment
first_sector:
    dq 0                ; Sectors numbers starts from 0: first sector = sector 0

HARD_DISK:
    db 0

BOOT_DISK:
    db 0

msg_loading:
    db 'loading stage 3',0

msg_loaded:
    db 'Stage 3 loaded',0

msg_sector_needed:
    db 'Sector needed: ',0

msg_sector_read:
    db 'Sector read: ',0

msg_err_lod_kernel:
    db 'Error loading kernel',0

msg_err_code:
    db 'Code ',0

err_code:
    times 6 db 0

times 1024 - ($-$$) db 0