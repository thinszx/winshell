#define _CRT_SECURE_NO_WARNINGS
/* 功能摘要
 *		尝试执行赋值语句，在读到一条命令并去除其中可能存在的if/while等关键词后，首先执行的就是本文件中内容
 * 功能说明
 *		返回值反映了该语句是否为变量的赋值语句，返回值为0，则是，为-1，则不是
 * 功能逻辑
 *		在进行赋值时，会维护一个hash表，将变量名称以及它们的值作为键值对放在hash表中
 *		根据返回值
 */
#include <stdio.h>
#include <ctype.h>
#include "varlib.h"
#include <Windows.h>

// 将$abc这样形式的字符串转变为$abc的值
int eval(char* src, char* dest, int len) {
	int	pos;			// dest指针
	char ch;			// 当前字符
	char* varval;		// 指向变量

	pos = 0;
	while (ch = *src++) {
		if (pos == len)			// 越界检查
			return -1;
		if (ch == '\0' || ch == EOF) {
			break;
		}
		// 本来觉得需要，后来发现在splitline.c里面已经做过转义了，这里的$一定是变量开头的，\$一定是转义
		if (ch == '\\')
			if (*src == '$')	// 将\进行转义
			{
				dest[pos++] = *src;
				src++;
				continue;
			}

		if (ch != '$')			// 不为变量字符
		{
			dest[pos++] = ch;	// 写入缓冲区
			continue;
		}

		/* 是$的情况 */
		varval = getvar(&src);	// 尝试读取变量名
		if (strlen(varval) + pos >= len)	// 越界检查
			return -1;

		strcpy(dest + pos, varval);		// 写入变量
		pos += strlen(varval);			// 移动指针
	}
	dest[pos] = '\0';
	return pos;
}

char* getvar(char** srcp) {
/*
 * 尝试读取变量的全称，在过程中检查合法性
 * 变量名称合法，返回变量名称
 * 否则返回$，并保持*srcp不动
 */
	char varname[VARLEN + 1];	// 变量名称
	int	pos;					// 写入缓冲区的指针位置
	char* cp;					// 用来生成变量的指针

	cp = *srcp;				
	pos = 0;

	while (is_an_idchar(*cp, pos))	// 当前字符合法
		varname[pos++] = *cp++;	
	varname[pos] = '\0';
	*srcp = cp;

	if (pos > 0) {	// 变量字符合法
		// cp = EVget(varname);
		cp = value_lookup(varname);
		//if (cp == NULL)		// 本身找不到时返回值就是""
		//	cp = (char*)"";
	}
	else
		cp = (char*)"$";			/* else return the $ */
	return cp;
}


int isaname(char* s) {
	/*
	 *	判定当前字符串是否是合法变量名
	 */
	int	pos = 0;
	while (*s && is_an_idchar(*s, pos++))
		s++;

	return ((!*s) && (pos != 0));
}

int is_an_idchar(char ch, int pos) {
	/*
	 *	判定当前字符是否可以位于合法变量名中
	 */
	return (isalpha(ch) || ch == '_' || (isdigit(ch) && pos > 0));
}
