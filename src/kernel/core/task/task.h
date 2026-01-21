#include <stdint.h>
#include <stddef.h>

enum task_state {
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCKED,
    TASK_SUSPENDED,
    TASK_ZOMBIE,
    TASK_NEW,
    TASK_TERMINATED
};

struct context {
    /* Registri generali */
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    
    /* Registri speciali */
    uint32_t sp;        // Stack Pointer
    uint32_t lr;        // Link Register
    uint32_t pc;        // Program Counter
    uint32_t cpsr;      // Current Program Status Register
    
    /* Puntatore alla base dello stack */
    void* stack_base;
    
    /* Dimensione dello stack */
    size_t stack_size;
};

typedef struct
{
    int pid;
    void* stack_ptr;
    enum task_state state;
    struct context context;
    struct task_t* next;
} task_t;