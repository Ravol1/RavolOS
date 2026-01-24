#include <stdint.h>
#include "paging.h"
#include "core/memory/memory.h"


#define PAGING_TABLES_ENTRIES 1024

typedef enum{
    PAGE_DIR_ATTRIBUTE_PRESENT = 0x01,
    PAGE_DIR_ATTRIBUTE_READONLY = 0x00,
    PAGE_DIR_ATTRIBUTE_READWRITE = 0x02,
    PAGE_DIR_ATTRIBUTE_SUPERVISOR = 0x00,
    PAGE_DIR_ATTRIBUTE_USER = 0x04,
    PAGE_DIR_ATTROBUTE_PWT = 0x08,
    PAGE_DIR_ATTRIBUTE_PCD = 0x10,
    PAGE_DIR_ATTRIBUTE_ACCESSED = 0x20,
    PAGE_DIR_ATTRIBUTE_DIRTY = 0x40,
    PAGE_DIR_ATTRIBUTE_4K = 0x00,
    PAGE_DIR_ATTRIBUTE_4M = 0x80,
} page_dir_attributes_t;



typedef enum{
    PAGE_DIR_ATTRIBUTE4M_GLOBAL = 0x0100,
    PAGE_DIR_ATTRIBUTE4M_PAT_AVAIL = 0x1000,
} page_dir_attributes_4M_t;


typedef enum{
    PAGE_TBL_ATTRIBUTE_PRESENT = 0x01,
    PAGE_TBL_ATTRIBUTE_READONLY = 0x00,
    PAGE_TBL_ATTRIBUTE_READWRITE = 0x02,
    PAGE_TBL_ATTRIBUTE_SUPERVISOR = 0x00,
    PAGE_TBL_ATTRIBUTE_USER = 0x04,
    PAGE_TBL_ATTROBUTE_PWT = 0x08,
    PAGE_TBL_ATTRIBUTE_PCD = 0x10,
    PAGE_TBL_ATTRIBUTE_ACCESSED = 0x20,
    PAGE_TBL_ATTRIBUTE_DIRTY = 0x40,
    PAGE_TBL_ATTRIBUTE_PAT_AVAIL = 0x80,
    PAGE_TBL_ATTRIBUTE_GLOBAL = 0x0100,
} page_table_attributes_t;



static uint32_t page_directory[PAGING_TABLES_ENTRIES] __attribute__((aligned(0x1000)));
static uint32_t first_page_table[PAGING_TABLES_ENTRIES] __attribute__((aligned(0x1000)));


extern void load_page_directory(uint32_t* page_dir);
extern void enable_paging(void);


void setup_stack_guard(){
    uint32_t guard_addr = (uint32_t)&_stack_guard;

    uint32_t index = guard_addr >> 12;
    
    if(guard_addr < 0x400000){
        first_page_table[index] = 0x00;
    }
}


void page_init(){
    for(int i = 0; i < PAGING_TABLES_ENTRIES; ++i){
        page_directory[i] = PAGE_DIR_ATTRIBUTE_READWRITE | PAGE_DIR_ATTRIBUTE_SUPERVISOR;
    }

    for(int i = 0; i < PAGING_TABLES_ENTRIES; ++i){
        first_page_table[i] = (i * 0x1000) | PAGE_TBL_ATTRIBUTE_PRESENT | PAGE_TBL_ATTRIBUTE_READWRITE | PAGE_TBL_ATTRIBUTE_SUPERVISOR; // i*0x1000 = address
    }

    page_directory[0] = ((uint32_t)first_page_table) | PAGE_DIR_ATTRIBUTE_PRESENT | PAGE_DIR_ATTRIBUTE_READWRITE | PAGE_DIR_ATTRIBUTE_SUPERVISOR;

    load_page_directory(page_directory);
    setup_stack_guard();
    enable_paging();
}

