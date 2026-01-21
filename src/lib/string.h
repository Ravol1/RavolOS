#ifndef STRING_H
#define STRING_H

#include "stdint.h"
#include "stddef.h"


int strcmp(const char* s1, const char* s2);
char* strtok(char *str, const char *delim);
char* strcpy(char* dest, const char* src);
char* strstr(const char *string1, const char *string2);

size_t len_str(char* str);
void inv_str(char* base, char* top);


#endif


