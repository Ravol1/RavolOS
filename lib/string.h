#pragma once

#include "stdint.h"
#include "stddef.h"

typedef struct{
    unsigned int len;
    char* data;
} string;

int strcmp(const char* s1, const char* s2);
char* strtok(char *str, const char *delim);
char* strcpy(char* dest, const char* src);
char* strstr(const char *string1, const char *string2);

size_t len_str(char* str);
void inv_str(char* base, char* top);
size_t find_char(string* src, char ch, size_t beg);
size_t find_substr(string* src, string* substr, size_t beg);
string new_str(char* data);
string new_str_buffer(size_t length);


