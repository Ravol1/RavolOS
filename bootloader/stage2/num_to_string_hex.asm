; ax = number to convert
; di = memory buffer where string will be saved
num_to_string_hex:
    mov byte [di], '0'
    inc di
    mov byte [di], 'x'
    inc di

    push di         ; Push address of first byte of output buffer
    mov bx, 0       ; Set counter for string lenght
    
    num_to_string_hex_loop:
        test ax, ax                 ;If previous division result was 0
            jz done_num_to_string_hex 

        
        mov dx, ax
        and dx, 0xF                 ; Gets remainder of ax/16
        shr ax, 4                   ; Divides ax by 16

        or dx, '0'                  ; Converts remainder to ASCII digit

        cmp dx, '9'
            jna below_ten

        
        add dx, ('A' - '9') - 1


        below_ten:

        mov [di], dl                ; Writes current digit on buffer
 
        inc bx                      ; Increases counter
        inc di                      ; Point to next byte in buffer

        jmp num_to_string_hex_loop


    done_num_to_string_hex:

        dec di                  ; Point to last valid byte
        pop si                  ; Retrieves adress of first character
        call invert_string

        ret

