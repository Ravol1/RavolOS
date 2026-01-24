#include "multiboot2.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


#define MB2_MAGIC 0x36D76289


bool mb2_is_magic(uint32_t magic){
    return (magic == MB2_MAGIC);
}


mb_tag_ptr mbi2_get_tag(uint8_t* mbi, multiboot_tag_type tag){
    uint32_t tot_size = *((uint32_t*)mbi);
    uint8_t* mbi_end = mbi + tot_size;

    for(uint8_t* ptr = mbi + 8; ptr < mbi_end; ){
        uint32_t* ptr32 = (uint32_t*)ptr;

        uint32_t found_type = ptr32[0];
        uint32_t size = ptr32[1];
        
        if(found_type != tag){
            ptr += (size + 7) & ~7;     // Alligned 8 bit
            continue;
        }

        mb_tag_ptr t;
        t.raw = ptr;
        return t;
    }

    mb_tag_ptr t;
    t.raw = NULL;
    return t;
}
