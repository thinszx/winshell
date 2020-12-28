#pragma once
#include<stdio.h>

char* readline(FILE* fp);
char** splitline(char* line);
char* createstr(char* s, int l);
void* malloc_s(size_t n);
void* realloc_s(void* p, size_t n);

/* 定义在exec.c中 */
wchar_t* char2wchar(char* text, size_t* converted);
wchar_t* generate_cmd(char* str);