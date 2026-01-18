;ax must contain number to convert
;di must point to memory buffer where string will be saved
num_to_string:
    push di         ;Push address of first byte of output buffer
    mov bx, 0       ;Set counter for string lenght
    
    num_to_string_div_loop:
        test ax, ax                 ;If previous division result was 0
            jz done_num_to_string 

        
        mov dx, 0                   ;Sets dx to 0 to correctly set dividend
        mov cx, 10                  ;Moves divisor to cx
        div cx                      ;Performs operation ax = dx:ax / cx; dx = remainder

        or dx, '0'                  ;Converts remainder to ASCII digit

        mov [di], dl                ;Writes current digit on buffer

        inc bx                      ;Increases counter
        inc di                      ;Point to nexy byte in buffer

        jmp num_to_string_div_loop


    done_num_to_string:
        dec di                  ;Point to last valid byte
        pop si                  ;Retrieves adress of first character
        call invert_string

        ret



