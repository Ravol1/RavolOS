#include "video.h"
#include "disk.h"
#include <stdlib.h>
#include "gdt.h"
#include "io.h"

#define KERNEL_SECTORS 64
#define KERNEL_START_SECTOR 0x800
#define KERNEL_START_ADDRESS 0x100000

extern void kernel_entry();


extern int main(){
    // Kernel location is at sector 0x800, mind that when you load kenel
    init_video();
    init_gdt();

    
    char* krn_addr = (char*)KERNEL_START_ADDRESS;
    
    if (ATA_read_sectors(0, KERNEL_START_SECTOR, KERNEL_SECTORS, krn_addr) != 0){
        printf("Error loading kernel...");
        while(1);
    }

    kernel_entry();

    return 0;
}
