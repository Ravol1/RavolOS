#include <stdint.h>
#include <stddef.h>


#define nullptr 0

#define MEM_SIZE        0x40000000              // 1GiB

#define BLOCK_SIZE      0x1000                  // 4 KiB
#define BLOCK_NUMBER    (MEM_SIZE/BLOCK_SIZE)

#define MEM_MAP_ADDRESS 0x300000                // 4 MiB 
#define MEM_MAP_LEN     (BLOCK_NUMBER/(sizeof(size_t) * 8))
#define MEM_MAP_SIZE    (MEM_MAP_LEN * sizeof(size_t))

#define MEM_START       (MEM_MAP_ADDRESS + (MEM_MAP_LEN * MEM_MAP_SIZE) + 0x1000)
#define MEM_TOP         (MEM_START + MEM_SIZE)


extern char* mem_pos;

void* malloc(size_t bytes);
void* memcpy(void* dest, void* src, size_t bytes);
void free(void* ptr);
void* realloc(void* data, size_t bytes);
void* memset(void* ptr, uint8_t value, size_t  count);


void init_mem();