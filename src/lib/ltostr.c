#include "ltostr.h"
#include "stddef.h"

static void invstr(const char* start, const char* end){
    for (char *start_cpy = (char*)start, *end_cpy = (char*)end; start_cpy < end_cpy; ++start_cpy, --end_cpy){
        char tmp = *start_cpy;
        *start_cpy = *end_cpy; 
        *end_cpy = tmp;
    }
}

void ltostr(long int num, int base, char* buff, size_t size){
    if(base < 0 || base > 36) return;

    char* ptr = buff;
    char* max_addr = ptr + size;  
    if(base <= 10){
        do{
            char remainder = num%base;
            num = num/base;

            (*ptr) = remainder | 0b00110000;    // Converts numerical value to corresponding ascii character

            ++ptr;            
            if(ptr >= max_addr - 1) return;

        }while(num != 0);
    }

    else{
        do{
            char remainder = num%base;
            num = num / base;

            remainder = remainder | 0b00110000;     // Converts numerical value to corresponding ascii character
            if(remainder > '9') remainder += ('A' - '9' - 1);
            *ptr = remainder;

            ++ptr;
            if(ptr >= max_addr -1) return;

        } while(num != 0);
    }

    (*ptr) = '\0';
    char* end = --ptr;
    invstr(buff, end);
    
}