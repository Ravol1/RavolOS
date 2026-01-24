section .text
global set_df_tss_cr3

set_df_tss_cr3:
    push ebp
    mov ebp, esp
    mov edx, [ebp + 8]

    mov eax, cr3
    mov [edx], eax

    pop ebp
    ret