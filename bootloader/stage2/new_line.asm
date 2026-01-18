new_line:
    pusha

    mov ah, 0x0e
    mov al, 0x0d                ;Carriage Return
    int 0x10

    mov al, 0x0a                ;Line Feed
    int 0x10
    
    popa
    ret