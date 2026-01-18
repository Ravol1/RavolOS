section .text



global load_idt
load_idt:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]

    lidt [eax]
    sti

    ;int 32
    ;int 33
    ;int 34
    ;int 35
    ;int 0x36
    ;int 0x37
    ;int 0x38
    ;int 0x39
    ;int 0x40
    ;int 0x41
    ;int 0x42
    ;int 0x43
    ;int 0x44
    ;int 0x45
    ;int 0x46
    ;int 0x47


    pop ebp
    ret



