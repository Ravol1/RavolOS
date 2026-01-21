#include "stddef.h"
#include "math.h" 

int atoi(const char* str){
    long long int num = 0;

    for (long i = 0; str[i] != '\0'; ++i){
        if(str[i] < '0' || str[i] > '9') return num;

        num += (str[i] - '0') * pow(10, i);
    }

    return num;
}

long int strtol(const char* nptr, char** endptr, int base){
    const char MAX_BASE = 36;

    long int num = 0;

    char* nptrcpy = (char*)nptr;

    if(base == 0){
        if(nptr[0] == '0'){
            switch (nptr[1]) {
                case 'b':
                    base = 2;
                    break;
                
                case 'x':
                    base = 16;
                    break;
                
                default:
                    break;
            }
        }

        else if('0' <= nptr[0] && nptr[0] <= '9'){
            base = 10;
        }

        else{
            *endptr = (char*)nptr;
            return 0;
        }
    }


    if (base == 2 && nptr[0] == '0' && nptr[1] == 'b') nptrcpy += 2;
    else if (base == 16 && nptr[0] == '0' && nptr[1] == 'x') nptrcpy += 2;


    if(base < 2 || base > MAX_BASE){
        *endptr = (char*)nptr;
        return 0;
    }

    if (base <= 10){
        char max = base - 1 + '0';
        
        size_t length = 0;
        for( ; '0' <= nptrcpy[length] && nptrcpy[length] <= max; ++length);
        *endptr = nptrcpy + length;
        --length;


        for(size_t i = 0; i <= length; ++i){
            num += (nptrcpy[length - i ] - '0') * pow(base, i);
        }
    }
    else{
        char max = base -1 + '0' + ('A' - '9') - 1;

        size_t length = 0;
        while(1){
            char c = nptrcpy[length];
            if((c < '0' || c > '9') && (c < 'A' || c > max) && (c < 'a' || c > max + ('a' - 'A'))) break;

            ++length;
        }
        
        *endptr = nptrcpy + length;
        --length;

        for(size_t i = 0; i <= length; i++){
            char c = nptrcpy[length  - i];
            long int inc = (c - '0');

            if (c > '9' && c <= 'Z') inc += ('9' - 'A' + 1);
            else if (c > 'Z' && c <= 'z') inc += ('9' - 'a' + 1);

            num += inc * pow(base, i);
        }
    }

    return num;
}