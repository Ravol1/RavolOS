#include "math.h"

long int pow(long int base, long int exp){
    if(exp == 0){
        return 1;
    }

    long int result = base;
    for (int i = 1; i < exp; i++){
        result*=base;
    }

    return result;
}