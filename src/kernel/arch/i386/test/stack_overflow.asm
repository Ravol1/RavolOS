section .text

global stack_overflow

stack_overflow:
    sub esp, 256
    call stack_overflow
    add esp, 256
    mov [0x2000], eax