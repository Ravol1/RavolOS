#include <stdint.h>

typedef struct {
    uint32_t gs, fs, ds, es;      
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; 
    uint32_t int_no, err_code;   
    uint32_t eip, cs, eflags;     
} registers_t;


void IRQ_print(uint8_t irq);
void interrupt_handler(registers_t *reg);
void exception_handler(registers_t *reg);
void irq_handler(registers_t *reg);