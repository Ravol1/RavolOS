;si must point to firt byte of the string
print_string:
    mov ah, 0x0e        ; Int 0x10 function to teletype output
    cld                 ; Sets direction flag to low (read forward)


    print_next_char:
        mov al, es:[si]
        inc si

        cmp al, 0                   ;Checks for null terminator
            je done_print_string        

        int 0x10                    ;BIOS interrupt for video services (ah = 0x0e -> print character)
        

        jmp print_next_char

    
    done_print_string:
        ret
