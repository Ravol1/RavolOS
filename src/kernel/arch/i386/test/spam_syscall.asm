section .text

global spam_syscall
extern kernel_wait_ms

SYSCALL_NUM equ 100
WAIT_TIME equ 125   ; ms


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
    int 46
    
    push dword WAIT_TIME
    call kernel_wait_ms
    add esp,4

    pop eax
    inc eax
    cmp eax, SYSCALL_NUM
    
    jl loop 
    
    popad
    ret