section .text

global spam_syscall


SYSCALL_NUM equ 11


spam_syscall:
    pushad
    
    mov eax, 0

    mov ebx, 5
    mov ecx, 9
    mov edx, 14
    mov esi, 2
    mov edi, 20
    mov ebp, 3
loop:
    push eax
    int 0x80
    pop eax

    inc eax
    cmp eax, SYSCALL_NUM
    jl loop 
 
    popad
    ret