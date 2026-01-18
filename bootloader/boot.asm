[bits 16]
[org 0x7c00]

STAGE_2_LOC equ 0x7e00  ;   After booting sector
STAGE_2_SECTORS equ 2

_main16:
    mov [BOOT_DISK], dl

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov bp, 0xe800
    mov sp, bp


    ; Set video mode (and clears screen)
    mov ah, 0x00
    mov al, 0x03
    int 0x10

    ; Hard Drive or floppy?
    cmp dl, 0x80
    jl floppy
    mov al, 1
    mov [HARD_DISK], al

    ; Set up disk packet
    mov word [sectors_to_load], STAGE_2_SECTORS 
    mov word [loading_segment], 0   
    mov word [loading_offset], STAGE_2_LOC

    mov word [first_sector], 1          ; Load from second sector (conting from 1)
    mov word [first_sector + 2], 0      ; Clear upper 6 bytes
    mov word [first_sector + 4], 0      ; Clear upper 6 bytes
    mov word [first_sector + 6], 0      ; Clear upper 6 bytes



    mov ah, 0x42                ; int 0x13 function to read sectors from drive
    mov dl, [BOOT_DISK]         ; Drive number
    mov si, disk_packet     
    int 0x13
    
    jc error_loading
    
    jmp after_loading
    

floppy:
    mov ah, 0x02                    ; Read sectors
    mov al, STAGE_2_SECTORS         ; How many sectors to read (1-128)
    mov ch, 0                       ; From cylinder 0
    mov cl, 2                       ; From sector 2 (conting from 1)
    mov dh, 0                       ; From head 0
    mov dl, [BOOT_DISK]             ; From boot disk
    mov bx, 0x7E00                  ; Where to load (0x7e00 = after booting sector)
    int 0x13

    jc error_loading



after_loading:
    mov byte dl, [BOOT_DISK]
    mov byte dh, [HARD_DISK]
    jmp STAGE_2_LOC
    jmp $


error_loading:
    mov si, msg_error_st2
    call print_string

    jmp $


BOOT_DISK: db 0
HARD_DISK: db 0         ; 0 = floppy, 1 = HDD

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

msg_error_st2:
    db 'Error loading stage 2.',0


%include "bootloader/stage2/print_string.asm"



times 446-($-$$) db 0


partition1:
    db 0x80                 ; Boot flag (0x80 = bootable, 0x00 = not bootable)
    db 0xFF, 0xFF, 0xFF     ; CHS start (dummy)
    db 0x0C                 ; Partition type (0x0C = FAT32 (LBA))
    db 0xFF, 0xFF, 0xFF     ; CHS end (dummy)
    dd 0x00000800           ; LBA start (sector 2048)
    dd 0x00100000           ; Size in sectors (512MB)

partition2:
    db 0x00                         ; Not bootable
    db 0xFF, 0xFF, 0xFF             ; (dummy)
    db 0x0C                         ; FAT32 (LBA)
    db 0xFF, 0xFF, 0xFF             ; (dummy)
    dd 0x00100801                   ; Start right after partition 1 
    dd 0xFFFFFFFF - 0x00100801      ; Size

partition3:
    db 0x00                 
    db 0x00, 0x00, 0x00     
    db 0x00                 
    db 0x00, 0x00, 0x00     
    dd 0x00000000          
    dd 0x00000000           

partition4:
    db 0x00                 ; Not bootable
    db 0x00, 0x00, 0x00     ; CHS start
    db 0x00                 ; Unused
    db 0x00, 0x00, 0x00     ; CHS end
    dd 0x00000000           ; LBA start
    dd 0x00000000           ; Size
    

    dw 0xAA55 ; Boot signature






