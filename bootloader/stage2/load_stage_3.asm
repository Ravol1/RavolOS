[bits 16]

STAGE_3_SECTORS equ 32
STAGE_3_LOC equ 0x8200


load_stage_3:
    mov si, msg_loading
    call print_string

    mov ax, [HARD_DISK]
    test ax, ax
    jz .use_standard_calls

    ; Set up disk packet
    mov word [sectors_to_load], STAGE_3_SECTORS
    mov word [loading_offset], STAGE_3_LOC
    mov word [first_sector], 3                      ; 4th sector, after stage 2
    mov word [first_sector + 2], 0                  ; Clear upper 6 bytes
    mov word [first_sector + 4], 0                  ; Clear upper 6 bytes
    mov word [first_sector + 6], 0                  ; Clear upper 6 bytes


    mov si, disk_packet
    mov ah, 0x42                ; int 0x13 function to read sectors from drive
    mov dl, [BOOT_DISK]         ; drive number
    int 0x13

    jc error_loading_disk
    jmp .done_load_stage_3


.use_standard_calls:
    mov ah, 0x0e
    mov al, 'H'
    int 0x10
    jmp $


    jc error_loading_disk


.done_load_stage_3:
    call new_line
    mov si, msg_loaded
    call print_string
    
    ret



error_loading_disk:
    mov si, msg_err_lod_kernel
    call print_string
    call new_line

    mov si, msg_err_code
    call print_string

    mov al, ah
    mov ah, 0
    mov di, err_code
    call num_to_string_hex
    mov si, err_code
    call print_string

    jmp $



