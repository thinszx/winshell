#pragma once
#include<stdio.h>
#include<Windows.h>

#define DEBUG

#define is_blank(x) ((x) == ' ' || (x) == '\t')
#define	MAXCMDLEN	512

enum states { NO_IF, SAW_IF, SAW_THEN, SAW_ELSE };	// �������̿��ƣ�����ǰָ��״̬��Ϊ���ֿ���
enum ok_to_execute { DONT_EXEC, OK_EXEC, SYN_ERR };			// ���̿��ƵĴ�����

/* ������splitline.c�� */
char* readline(FILE* fp);
char** splitline(char* line);
char* createstr(char* s, int l);
void* malloc_s(size_t n);
void* realloc_s(void* p, size_t n);
void fatal(char* s1, char* s2, int n);

/* ������exec.c�� */
wchar_t* char2wchar(char* text, size_t* converted);
wchar_t* generate_cmd(char* str);

/* ������controlflow.c�У��������̿��� */
int execute_check();
int is_control_command(char* s);
int do_control_command(char** args);
int syn_err(char* msg);

int read_if_block(FILE* fp);
char** read_loop_block(FILE* fp);
int check_loop(char* varname);
int set_loop_condition(char* varname);
int execute_loop(char** loop_block);

/* ������controlflow2.c�У��������̿��� */
int if_check(char* cmd);
int save_env();
int restore_env();
void set_ifenv();

/* ������process.c�У���������ִ�� */
DWORD process(char* args);
DWORD execute(char** args);
char* concatenate_char_array(char** arr);

/* ������eval.c */
// char* getvar(char** srcp);
int check_inside(char* cmd);