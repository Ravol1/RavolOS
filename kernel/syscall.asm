section .text


global get_syscall_params


get_syscall_params:
    push ebp
    push edi
    push esi
    push edx
    push ecx
    push ebx
    push eax

    mov eax, esp

    add esp, 28

    ret
