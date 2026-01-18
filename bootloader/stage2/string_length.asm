;si -> pointer to first byte of string
;di -> pointer to where to save string length
string_length:
    pusha

    cld                 ;Sets direction flag to low (read forward)               
    mov bx, 0           ;Sets lenght counter

    string_length_loop:
        lodsb                       ;Loads the next byte from the string into AL

        test al, al                 ;Checks for null terminator
            jz done_string_length

        inc bx
        jmp string_length_loop



    done_string_length:
        mov [di], bx

        popa
        ret