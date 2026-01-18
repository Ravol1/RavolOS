;Performs operation *di = ax^bx
power:
    pusha

    cmp bx, 0
        jz exponent_zero

    mov cx, ax              ;Saves base to cx
    
    power_loop:
        dec bx

        test bx, bx
            jz done_power

        mul cx

        jmp power_loop


    exponent_zero:
        mov ax, 1

    done_power:
        mov [di], ax
        
        popa
        ret