; si = first byte of the string buffer
; di = last byte of the string buffer (excluding null terminator)
invert_string:
    cmp si, di
    jnb done_invert_string

    mov al, [si]
    mov ah, [di]

    mov [si], ah
    mov [di], al

    inc si
    dec di

    jmp invert_string


    done_invert_string:
        ret
