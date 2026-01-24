#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "memory.h"
#include "boot/multiboot2.h"
#include "drivers/video/video.h"


#define HMA_BARRIER 0x100000
#define MAX_MMAP_ENTRIES 64


typedef struct 
{
    uint32_t base;
    uint32_t size;
    uint8_t status;
} mmap_entry_t;


uint32_t mmap_size;
mmap_entry_t mmap[MAX_MMAP_ENTRIES];  


typedef struct
{
    size_t blocks;

} memory_header_t;

bool mem_initialized = false;

size_t* mem_map = (size_t*)MEM_MAP_ADDRESS;          

int8_t map_set(size_t bit, bool value){
    if(bit >= BLOCK_NUMBER){
        return -1;
      }

    size_t map_index = bit/(sizeof(size_t)*8);

    size_t bit_offset = bit % (sizeof(size_t)*8);

    if(value == true){
        mem_map[map_index] = mem_map[map_index] | (1 << bit_offset);
    }


    else{
        mem_map[map_index] = mem_map[map_index] & (~(1 << bit_offset ));

    }

    return 0;
}

int8_t map_get(size_t bit){
    if(bit >= BLOCK_NUMBER){
        return -1;
    }

    size_t map_index = bit/(sizeof(size_t)*8);
    size_t map_block = mem_map[map_index];

    size_t bit_offset = bit % (sizeof(size_t)*8);

    return (map_block >> bit_offset) & 1;
}



void* malloc(size_t bytes){
    if(!mem_initialized){
        return nullptr;
    }
    
    if(bytes > MEM_SIZE || bytes == 0){
        return nullptr;
    }
    
    
    size_t total_size = bytes + sizeof(memory_header_t);

    size_t need_block = total_size/BLOCK_SIZE;
    if(bytes%BLOCK_SIZE){
        need_block++;
    }

    if(need_block > BLOCK_NUMBER){
        return nullptr;
    }


    for(unsigned long int i = 0; i<BLOCK_NUMBER; i++){      
        int8_t bit_value = map_get(i);
        
        if (bit_value != -1 && !bit_value){
            bool valid_location = true;
            
            if (i + need_block >= BLOCK_NUMBER){
                break;
            }

            for(unsigned long int j = i+1; j < i+need_block; j++){
                int8_t bit_value = map_get(j);
                if (bit_value == -1 || bit_value){
                    valid_location = false;
                    break;
                }
            }

            if(valid_location){

                for(int j = i; j < i+need_block; j++){
                    if(map_set(j, true) == -1){
                        return nullptr;
                    }
                }

                memory_header_t* header = (memory_header_t*)(MEM_START + i * BLOCK_SIZE);
                header->blocks = need_block;
            
                return (void*)((char*)header + sizeof(memory_header_t));
            }

        }
    }

    return nullptr;
    
}

void* memcpy(void* dest, void* src, size_t bytes){   
    uint8_t* end = (uint8_t*)src + bytes;

    for(uint8_t* op = (uint8_t*)src, *dp = (uint8_t*)dest; 
        op < end; ++op, ++dp){
        
        *dp = *op;
    }
    
    return dest;
}

void free(void* ptr){
    memory_header_t* header = (memory_header_t*)ptr - sizeof(memory_header_t);
    size_t blocks = header->blocks;


    size_t start_block = ((int8_t*)ptr - (int8_t*)MEM_START) / BLOCK_SIZE;

    for(uint32_t i = start_block; i<start_block+blocks; i++){
        map_set(i, false);
    }
}

void* realloc(void* data, size_t bytes){
    if(!mem_initialized){
        return nullptr;
    }

    if(data == nullptr){
        return malloc(bytes);
    }

    if(bytes == 0){
        free(data);
        return nullptr;
    }


    memory_header_t* header = (memory_header_t*)data - sizeof(memory_header_t);
    size_t origial_blocks = header->blocks;


    size_t req_blocks = (bytes + sizeof(memory_header_t))/BLOCK_SIZE;

    if((bytes + sizeof(memory_header_t))%BLOCK_SIZE != 0){
        req_blocks++;
    }

    void* out = malloc(bytes);

    size_t copy_size = 0;

    if(req_blocks < origial_blocks){
        copy_size = bytes;
    }

    else{
        copy_size = origial_blocks*BLOCK_SIZE - sizeof(memory_header_t);
    }


    memcpy(out, data, copy_size);

    free(data);
    return out;
}

void* memset(void* ptr, uint8_t value, size_t count){
    void* ret = ptr;

    for(; ptr < (void*)((size_t)ret + count); ptr++)
        *((char*)ptr) = value;

    return ret;
}


mem_init_status mem_init(uint8_t* mbi) {

}