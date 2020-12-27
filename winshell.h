#pragma once
#include<stdio.h>

char* readline(FILE* fp);
char** splitline(char* line);
char* createstr(char* s, int l);
void* malloc_s(size_t n);
void* realloc_s(void* p, size_t n);