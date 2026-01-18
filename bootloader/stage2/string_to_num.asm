;si -> pointer to input string
;di -> pointer to where to save number
string_to_num:
    pusha

    cld                 ;Sets direction flag to low (read forward) 

    mov cx, di                  ;Saves output address
    mov di, len_str
    call string_length
    mov di, cx                  ;Retrieves output address
    
    mov bx, [len_str]       ;Initialize counter
    dec bx                  ;For iterating from len-1 to 0
    mov cx, 0               ;Initialize output

    string_to_num_loop:       
        lodsb                   ;Loads next byte of string in al

        test al, al             ;Check fo null terminator
            jz done_string_to_num

        sub al, '0'             ;ASCII digit to numeric value

        mov dx, ax
        push di

        mov di, pow_result
        mov ax, 10
        call power              ;Calculate 10^bx (10^counter)
        
        pop di
        mov ax, dx


        mov ah, 0               ;Makes sure that ax = al
        mov dx, 0               ;Sets dx to 0 to correctly perform multiplication

        mul word [pow_result]   ;Performs dx:ax = ax * *pow_result = ax * 10^bx


        add cx, ax


        dec bx                  ;Increment counter

        jmp string_to_num_loop


    done_string_to_num:
        mov [di], cx

        popa
        ret
        



len_str:
    dw 0

pow_result:
    dw 0


num_status:
    times 20 db 0