#include <stdint.h>

typedef struct {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
} __attribute__((packed)) syscall_regs_caller;

extern void system_call(syscall_regs_caller* regs);