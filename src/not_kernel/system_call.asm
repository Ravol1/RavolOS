section .text
global system_call

system_call:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]

    mov ebx, [eax + 4]
    mov ecx, [eax + 8]
    mov edx, [eax + 12]
    mov esi, [eax + 16]
    mov edi, [eax + 20]
    mov eax, [eax]
    
    int 0x80
    
    pop ebp
    ret