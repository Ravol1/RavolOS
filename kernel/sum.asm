section .text
global add_numbers

add_numbers:
    push ebp
    mov ebp, esp

    mov eax, [ebp+8] 
    add eax, [ebp+12]

    pop ebp
    ret