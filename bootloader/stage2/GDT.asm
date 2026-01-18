GDT_Start:
    null_descriptor:
        dq 0    

    kernel_code_segment:
        dw 0xffff       ; lower 16 bits of limit
        dw 0            ; 16 bits +
        db 0            ; 8 bits = 24 -> Lower 24 bits of the base (32 bit)     (Address 0x100000)
        db 10011010b    ; Access byte

        db 11111100b    ; 4 upper bits of limit + flags
        db 0            ; Higher 8 bits of the base (32 bit)

    kernel_data_segment:
        dw 0xffff
        dw 0
        db 0x0
        db 10010010b
        db 11111100b
        db 0

    user_code_segment:
        dq 0
    
    user_data_segment:
        dq 0

    task_state_segment:
        dq 0

    video_descriptor:
    dw 0xFFFF       
    dw 0x8000           
    db 0x0B             
    db 10010010b       
    db 11001111b        
    db 0                


GDT_End:


GDT_Descriptor:
    dw GDT_End - GDT_Start - 1      ;Size
    dd GDT_Start                    ;Start

    KER_CODE_SEG equ kernel_code_segment - GDT_Start
    KER_DATA_SEG equ kernel_data_segment - GDT_Start
    USR_CODE_SEG equ user_code_segment - GDT_Start
    USR_DATA_SEG equ user_data_segment - GDT_Start
    TSK_STATE_SEG equ task_state_segment - GDT_Start
    VIDEO_SEG equ video_descriptor - GDT_Start