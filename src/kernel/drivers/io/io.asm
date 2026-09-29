section .text
global inb
global outb
global inw
global outw
global indw
global outdw

inb:
    push ebp
    mov ebp, esp

    xor eax, eax

    mov dx, [ebp+8]     ; Port
    
    in al, dx           ; al <- byte read from port dx 

    pop ebp
    ret


outb:
    push ebp
    mov ebp, esp
    
    mov dx, [ebp+8]     ; Port 
    mov al, [ebp+12]    ; Value

    out dx, al      ; port dx <- al

    pop ebp
    ret


inw:
    push ebp
    mov ebp, esp

    xor eax, eax

    mov dx, [ebp+8]     ; Port
    
    in ax, dx           ; ax <- word read from port dx 

    pop ebp
    ret


outw:
    push ebp
    mov ebp, esp
    
    mov dx, [ebp+8]     ; Port 
    mov ax, [ebp+12]    ; Value

    out dx, ax      ; port dx <- ax

    pop ebp
    ret

indw:
    push ebp
    mov ebp, esp


    mov dx, [ebp+8]     ; Port
    
    in eax, dx          ; eax <- double word read from port dx 

    pop ebp
    ret

outdw:
    push ebp
    mov ebp, esp
    
    mov dx, [ebp+8]     ; Port 
    mov ax, [ebp+12]    ; Value

    out dx, eax      ; port dx <- eax

    pop ebp
    ret
