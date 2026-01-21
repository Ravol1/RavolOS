section .text

KERNEL_DATA_SELECTOR equ 0x10


extern interrupt_handler


global dummy_test_handler
dummy_test_handler:
    iret



%macro EXCEPTION_NO_CODE 1
global exception%1
exception%1:
    cli
    push dword 0    ; Dummy error code to use the same handler
    push dword %1
    jmp interrupt_common
%endmacro


%macro EXCEPTION_CODE 1
global exception%1
exception%1:
    cli
    push dword %1
    jmp interrupt_common
%endmacro

%macro IRQ 1
global irq%1
irq%1:
    cli
    push dword 0
    push dword %1 + 0x20
    jmp interrupt_common
%endmacro

%macro INT 1
global int%1
int%1:
    cli
    push dword 0
    push dword %1
    jmp interrupt_common
%endmacro

interrupt_common:
    pushad       ; Pushes EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI (8 regs × 4 bytes = 32 bytes)

    push es
    push ds
    push fs
    push gs      ; Total: 4 regs × 4 bytes = 16 bytes

    mov ax, KERNEL_DATA_SELECTOR
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; STACK LAYOUT at this point:
    ; ───────────────────────────────────────────────────────────────────────────
    ; [ESP + 0]     GS, FS, DS, ES      (4 regs × 4 bytes = 16 bytes)
    ; [ESP + 16]    PUSHAD registers    (8 regs × 4 bytes = 32 bytes)
    ; [ESP + 48]    Interrupt Number    (Pushed by macro)
    ; [ESP + 52]    Error Code          (Pushed by CPU or dummy)
    ; [ESP + 56]    EIP, CS, EFLAGS     (Pushed by CPU on exception)



    push esp
    call interrupt_handler
    add esp, 4 

    pop gs
    pop fs
    pop ds
    pop es
    popad

    add esp, 8               ; Remove exception number and error code that were pushed by the macro

    iret


EXCEPTION_NO_CODE 0   ; #DE - Divide Error - Fault
EXCEPTION_NO_CODE 1   ; #DB - Debug Exception - Fault/Trap
EXCEPTION_NO_CODE 2   ; _   - NMI Interrupt // Maybe map it as an iterrupt?
EXCEPTION_NO_CODE 3   ; #BP - Breakpoint - Trap
EXCEPTION_NO_CODE 4   ; #OF - Overflow - Trap
EXCEPTION_NO_CODE 5   ; #BR - BOUND Rance Exceeded - Fault
EXCEPTION_NO_CODE 6   ; #UD - Undefined Opcode - Fault
EXCEPTION_NO_CODE 7   ; #NM - Device Not Available (No Math Coprocessor) - Fault
EXCEPTION_CODE 8      ; #DF - Double Fault - Abort
EXCEPTION_NO_CODE 9   ; Coprocessor Segment Overrun - Reserved - Fault- Unused after the Intel386 
EXCEPTION_CODE 10     ; #TS - Invalid TTS - Fault
EXCEPTION_CODE 11     ; #NP - Segment Not Present - Fault
EXCEPTION_CODE 12     ; #SS - Stack-Segment Fault - Fault
EXCEPTION_CODE 13     ; #GP - General Protection - Fault
EXCEPTION_CODE 14     ; #PF - Page Fault - Fault
EXCEPTION_NO_CODE 15  ; Intel Reserved - Fault
EXCEPTION_NO_CODE 16  ; #MF - x87 FPU Flating-Point Error (Math Fault) - Fault
EXCEPTION_CODE 17     ; #AC - Alignment Check - Fault - From the Intel486 
EXCEPTION_NO_CODE 18  ; #MC - Machine Check - Abort - From the Pentium family
EXCEPTION_NO_CODE 19  ; #XM - Sexception_commonIMID Floating-Point Exception - Fault - From the Pentium III
EXCEPTION_NO_CODE 20  ; #VE - Virtualization Exception - Fault
EXCEPTION_CODE 21     ; #CP - Control Protection Exception - Fault
EXCEPTION_NO_CODE 22  ; Intel Reserver
EXCEPTION_NO_CODE 23  ; Intel Reserver
EXCEPTION_NO_CODE 24  ; Intel Reserver
EXCEPTION_NO_CODE 25  ; Intel Reserver
EXCEPTION_NO_CODE 26  ; Intel Reserver
EXCEPTION_NO_CODE 27  ; Intel Reserver
EXCEPTION_NO_CODE 28  ; Intel Reserver
EXCEPTION_NO_CODE 29  ; Intel Reserver
EXCEPTION_CODE 30  ; Intel Reserver
EXCEPTION_NO_CODE 31  ; Intel Reserver

IRQ 0       ; System timer
IRQ 1       ; Keyboard controller
IRQ 2       ; Slave PIC
IRQ 3       ; Second RS-232 serial port
IRQ 4       ; First RS-232 serial port
IRQ 5       ; Parallel por 3 and 3 OR soundcard
IRQ 6       ; Floppy disl controller
IRQ 7       ; First parallel port
IRQ 8       ; Real time clock
IRQ 9       ; Open interrupt
IRQ 10      ; Open interrupt
IRQ 11      ; Open interrupt
IRQ 12      ; PS/2
IRQ 13      ; Math coprocessor
IRQ 14      ; Primary ATA
IRQ 15      ; Secondary ATA

INT 128     ; Syscall (0x80)
    
