#pragma once
#include<stdio.h>
#include<Windows.h>

#define DEBUG

/* 定义在splitline.c中 */
char* readline(FILE* fp);
char** splitline(char* line);
char* createstr(char* s, int l);
void* malloc_s(size_t n);
void* realloc_s(void* p, size_t n);
void fatal(char* s1, char* s2, int n);

/* 定义在exec.c中 */
wchar_t* char2wchar(char* text, size_t* converted);
wchar_t* generate_cmd(char* str);

/* 定义在controlflow.c中，用作流程控制 */
int execute_check();
int is_control_command(char* s);
int do_control_command(char** args);
int syn_err(char* msg);

/* 定义在process.c中，用作进程执行 */
DWORD process(char** args);
DWORD execute(char** args);
char* concatenate_char_array(char** arr);