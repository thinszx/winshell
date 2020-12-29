/*
* controlflow.c - 用来进行流程控制
*/
#include<stdio.h>
#include<string.h>
#include "winshell.h"

enum states {NEUTRAL, WANT_THEN, THEN_BLOCK};	// 将流程控制分为三个分区
enum results {SUCCESS, FAIL};

static int if_state = NEUTRAL;
static int if_result = SUCCESS;
static int processed_stat = 0;	// 流程控制语句中进程执行的状态

// 通过当前的if_state判断是否可以进行语句的执行任务
int execute_check() {
	int rv = 0;	// 检查结果

	if (if_state == WANT_THEN)	// if后面本应紧跟then却未出现
		rv = syn_err((char*)"then expected");	// rv == -1
	else if (if_state == THEN_BLOCK && if_result == FAIL)	// 未成功执行条件语句
		rv = -1;
	// 其余情况或中立区，直接执行命令即可，无需判断
	return rv;
}

// 判断是否为流程控制语句
int is_control_command(char* s) {
	return (strcmp(s, "if") == 0
		|| strcmp(s, "then") == 0
		|| strcmp(s, "fi") == 0
		);
}

// 根据当前if_state的状态进行条件语句的执行
int do_control_command(char** args) {
	char* ctrlflag = args[0];
	int rv = -1;	// 条件语句执行成功标志，默认为不成功

	if (strcmp(ctrlflag, "if") == 0) {
		if (if_state != NEUTRAL)	// 当前不在中立区当中
			rv = syn_err((char*)"if unexpected");
		else {
			processed_stat = process(args + 1);	// 指向流程控制语句后的第一部分
			if_result = (processed_stat == 0 ? SUCCESS : FAIL);
			if_state = WANT_THEN;	// 期望then语句，shell中IF后面必须跟then语句
			rv = 0;	// 返回成功执行标志
		}
	}
	else if (strcmp(ctrlflag, "then") == 0) {
		if (if_state != WANT_THEN)	// 当前不在WANT_THEN区域中却出现了then
			rv = syn_err((char*)"then unexpected");
		else {
			if_state = THEN_BLOCK;
			rv = 0;
		}
	}
	else if (strcmp(ctrlflag, "fi") == 0) {
		if (if_state != THEN_BLOCK)
			rv = syn_err((char*)"fi unexpected");
		else {
			if_state = NEUTRAL;
			rv = 0;
		}
	}
	else
		fatal((char*)"internal error processing: ", ctrlflag, 2);
	return rv;
}

int syn_err(char* msg) {
	if_state = NEUTRAL;
	fprintf_s(stderr, "syntax error: %s\n", msg);
	return -1;
}