#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include <stdio.h>
#include "winshell2.h"
#include "varlib.h"

enum bigsmall {SMALLER, LARGER, EQUAL, EQUALSMALL, EQUALLRAGE} finalstate;	// 针对左边相较于右边的情况
static char* var;
static int finalnum = 0;

int check_loop(char* varname) {
	switch (finalstate)
	{
	case SMALLER:
		return atoi(value_lookup(varname)) < finalnum ? 0 : 1;
	case LARGER:
		return atoi(value_lookup(varname)) > finalnum ? 0 : 1;
	case EQUAL:
		return atoi(value_lookup(varname)) == finalnum ? 0 : 1;
	case EQUALSMALL:
		return atoi(value_lookup(varname)) <= finalnum ? 0 : 1;
	case EQUALLRAGE:
		return atoi(value_lookup(varname)) >= finalnum ? 0 : 1;
	default:
		break;
	}
}

//int set_loop_condition(char* varname, const int num, bigsmall state) {
//	finalnum = num;
//	finalstate = state;
//	// 检验是否存在这个变量
//	if (value_lookup(varname) == "") 
//		fatal((char*)"no such a variabale", varname, 2);
//	// 本来就满足的话，返回1，证明根本就不用执行循环
//	return check_loop(varname);
//}
int set_loop_condition(char* cmd) {
	char** tmplist = splitline(cmd);
	//if (tmplist[1] == "-eq")
	//	finalstate = EQUAL;
	//if (tmplist[1] == "-lt")
	//	finalstate = SMALLER;
	//if (tmplist[1] == "-le")
	//	finalstate = EQUALSMALL;
	//if (tmplist[1] == "-gt")
	//	finalstate = LARGER;
	//if (tmplist[1] == "-ge")
	//	finalstate = EQUALLRAGE;
	if (strstr(cmd, "-eq") != NULL)
		finalstate = EQUAL;
	if (strstr(cmd, "-lt") != NULL)
		finalstate = SMALLER;
	if (strstr(cmd, "-le") != NULL)
		finalstate = EQUALSMALL;
	if (strstr(cmd, "-gt") != NULL)
		finalstate = LARGER;
	if (strstr(cmd, "-ge") != NULL)
		finalstate = EQUALLRAGE;
	char* varname = (char*)malloc_s(MAXCMDLEN);
	char* num = (char*)malloc_s(MAXCMDLEN);
	// for (int i = 0; i < strlen(tmplist[0]) - 2; i++)
	memset(varname, 0, MAXCMDLEN);
	memset(num, 0, MAXCMDLEN);
	strcpy(varname, tmplist[0] + 2);	// 把[$隔过去
		// varname[i] = tmplist[0][i+2];	// 把[$隔过去
	// for (int i = 0; i < strlen(tmplist[2]) - 1; i++)
	// num[i] = tmplist[2][i];
	memcpy(num, tmplist[2], (strlen(tmplist[2]) - 1));
	var = varname;
	finalnum = atoi(num);
	// 检验是否存在这个变量
	if (value_lookup(var) == "")
		fatal((char*)"no such a variabale", var, 2);
	// 本来就满足的话，返回1，证明根本就不用执行循环
	return check_loop(var);
}

// 读取循环块
char** read_loop_block(FILE* fp) {
	int linenum = 0;
	int bufsize;
	int len = 0;
	char** lines = (char**)malloc_s(BUFSIZ);
	memset(lines, 0, BUFSIZ);
	char* buf = readline(fp);
	if (strcmp(buf, "do") != NULL)
		fatal((char*)"wrong flag in loop", (char*)"no \"do\" command", 2);
	bufsize = BUFSIZ;
	while (TRUE) {
		char* newline = readline(fp);
		if (newline == NULL)
			fatal((char*)"no done flag in loop", (char*)"please check shell script", 2);
		if (strcmp(newline, "done") == NULL)	// done不需要加进去
			return lines;
		lines[linenum] = (char*)malloc_s(BUFSIZ);
		memset(lines[linenum], 0, BUFSIZ);
		lines[linenum] = newline;

		// buf++;

		linenum++;
		if (linenum >= bufsize) {
			lines = (char**)realloc_s(lines, linenum + BUFSIZ);
			bufsize += BUFSIZ;
			// linenum += (BUFSIZ / sizeof(char**));
		}
	}
}

int execute_loop(char** loop_block) {
	//char** line = (char**)malloc_s(sizeof(loop_block));
	//memset(line, 0, sizeof(loop_block));
	char** line = loop_block;
	//while (*ptr != NULL) {
	//	*ptr = (char*)malloc_s(sizeof(*ptr));
	//	strcpy(*line, *ptr);
	//	line++;
	//	ptr++;
	
	//char** line = (char**)malloc_s(sizeof(loop_block));
	//int i = 0;
	//memset(line, 0, sizeof(loop_block));
	//while (loop_block[i] != NULL) {
	//	*line = malloc_s(BUFSIZ);
	//	*line = loop_block[i];
	//	memcpy(*line, loop_block[i], sizeof(loop_block[i]));
	//	line++;
	//}*loop_block
	while (TRUE) {	// 这个部分和exec.c里面的逻辑基本都是一样的
		while (*line != NULL) {
			// 尝试进行变量的赋值*line[0]
			// 防止echo a=$a这样的代码出现，如果等号前有空格，那么一定不是赋值语句
			if ((strncmp(*line, "if ", 3) != 0) && (strncmp(*line, "else ", 5) != 0) &&
				(strncmp(*line, "while ", 6) != 0) && (strncmp(*line, "elif ", 5) != 0) && (check_inside(*line) != 1)) {
				int assigned = value_assign(*line);
				if (assigned == 0) {
					line++;
					continue;
				}
			}

			if (*line == '\0') {   // 空行的情况
				line++;
				continue;
			}
			char newstr[MAXCMDLEN];
			if (eval(*line, newstr, MAXCMDLEN) == -1) {
				fprintf(stderr, "line too long\n");
				return 1;
			}
			// printf("%s", newstr);
			char** args = splitline(newstr);
			if (if_check(newstr) == OK_EXEC)
				process(newstr);   // 执行语句
				line++;
		}
		if (check_loop(var) == 1)
			return 1;
		//char** line = (char**)malloc_s(sizeof(loop_block));
		//char** ptr = loop_block;
		//while (*ptr != NULL) {
		//	*line = (char*)malloc_s(sizeof(*ptr));
		//	strcpy(*line, *ptr);
		//	ptr++;
		//}
		line = loop_block;
		// memcpy(line, loop_block, sizeof(loop_block));	// line重新回到原点
	}

}