#pragma once
#include<stdio.h>
#include<Windows.h>

#define DEBUG

#define is_blank(x) ((x) == ' ' || (x) == '\t')
#define	MAXCMDLEN	512

enum states { NO_IF, SAW_IF, SAW_THEN, SAW_ELSE };	// 用作流程控制，将当前指令状态分为多种可能
enum ok_to_execute { DONT_EXEC, OK_EXEC, SYN_ERR };			// 流程控制的处理结果

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

int read_if_block(FILE* fp);
char** read_loop_block(FILE* fp);
int check_loop(char* varname);
int set_loop_condition(char* varname);
int execute_loop(char** loop_block);

/* 定义在controlflow2.c中，用作流程控制 */
int if_check(char* cmd);
int save_env();
int restore_env();
void set_ifenv();

/* 定义在process.c中，用作进程执行 */
DWORD process(char* args);
DWORD execute(char** args);
char* concatenate_char_array(char** arr);

/* 定义在eval.c */
// char* getvar(char** srcp);
int check_inside(char* cmd);