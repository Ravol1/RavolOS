#include <stdint.h>
#include <stddef.h>


#define nullptr 0


extern char _kernel_start[];
extern char _kernel_end[];
#define KERNEP_LENGTH (_kernel_start - _kernel_end)

extern char _code_start[];
extern char _code_end[];

extern char _rodata_start[];
extern char _rodata_end[];

extern char _data_start[];
extern char _data_end[];

extern char _bss_start[];
extern char _bss_end[];

extern char _stack_guard[];
extern char _stack_bottom[];
extern char _stack_top[];


//CHANGE ALL THIS WITH MEMORY MAPPING
#define MEM_SIZE        0x40000000              

#define BLOCK_SIZE      0x1000                  // 4 KiB
#define BLOCK_NUMBER    (MEM_SIZE/BLOCK_SIZE)

#define MEM_MAP_ADDRESS 0x300000                // 4 MiB 
#define MEM_MAP_LEN     (BLOCK_NUMBER/(sizeof(size_t) * 8))
#define MEM_MAP_SIZE    (MEM_MAP_LEN * sizeof(size_t))

#define MEM_START       (MEM_MAP_ADDRESS + (MEM_MAP_LEN * MEM_MAP_SIZE) + 0x1000)
#define MEM_TOP         (MEM_START + MEM_SIZE)



typedef enum {
    MEM_INIT_OK = 0,
    MEM_INIT_NO_MB2_MMAP,
    MEM_INIT_MEM_TRUNC
} mem_init_status;


extern char* mem_pos;

void* malloc(size_t bytes);
void* memcpy(void* dest, void* src, size_t bytes);
void free(void* ptr);
void* realloc(void* data, size_t bytes);
void* memset(void* ptr, uint8_t value, size_t  count);


mem_init_status mem_init();