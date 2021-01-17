#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include "varlib.h"
#include "winshell2.h"

#define	MAXVARS	200	// 存储变量的最大数量

struct var {
	char* str;		// str的格式为a=2
	int  global;	// 是否为全局变量，暂时没用
};

static struct var table[MAXVARS];	// 存储变量的table

static char* new_string(char*, char*);
static struct var* find_item(char*, int);

int value_store(char* name, char* val) {
	/*
	 * 遍历列表，如果找到，便置换该变量值，否则则将变量添加到末尾
	 * 不支持删除变量！！！@TODO
	 * 返回值为1，则意为报错；返回值为0，则意为正常执行
	 */
	struct var* itemp;
	char* s;
	int	rv = 1;

	// 查找变量并重新为变量赋值
	if ((itemp = find_item(name, 1)) != NULL && (s = new_string(name, val)) != NULL)
	{
		if (itemp->str)			// 变量存在
			free(itemp->str);	// 删除该变量的值
		itemp->str = s;			// 存在时重新为变量赋值，不存在时创建变量值
		rv = 0;
	}
	return rv;
}

char* new_string(char* name, char* val) {
	/*
	 * 返回一个name=value格式的字符串
	 */
	char* retval;

	retval = (char*)malloc_s(strlen(name) + strlen(val) + 2);
	if (retval != NULL)
		sprintf_s(retval, strlen(name) + strlen(val) + 2, "%s=%s", name, val);
	return retval;
}

char* value_lookup(char* name) {
	/*
	 * 若查到变量，则返回变量的值
	 * 否则返回空字符串
	 */
	struct var* itemp;

	if ((itemp = find_item(name, 0)) != NULL)
		return itemp->str + 1 + strlen(name);	// 指针移到变量名和等号后
	return (char*)"";

}

int value_assign(char* cmd) {
	/* 尝试进行变量赋值，若找不到等号，则证明不是变量赋值语句，返回-1 */
	const char sep = '=';
	char** dest = (char**)malloc_s(BUFSIZ);
	//while (*dest != NULL)
	//	*dest++ = (char*)malloc_s(BUFSIZ);
	int num = 0;
	split(cmd, &sep, dest, &num);
	// printf_s("%s, %s", dest[0], dest[1]);
	if (num != 2)
		return -1;
	else {
		if (strstr((const char*)dest[1], (const char*)"+") != NULL) {	// 只实现了加法且没有检查加法的合法性，不然工程量太庞大了...
			char** tmp = (char**)malloc_s(8);
			int add_num = 0;
			int sum = 0;
			char newstr[MAXCMDLEN];
			if (eval(dest[1], newstr, MAXCMDLEN) == -1) {
				fprintf(stderr, "line too long\n");
				return 1;
			}
			split(newstr, (const char*)"+", tmp, &add_num);
			for (int i = 0; i < add_num; i++)
				sum += atoi(tmp[i]);
			// system(newstr);
			char str[25];
			return value_store(dest[0], _itoa(sum, str, 10));
		}
		return value_store(dest[0], dest[1]);
		// printf_s("%s, %s", dest[0], dest[1]);
		// return 1;
	}
}

void split(char* src, const char* separator, char** dest, int* num) {
	/*
		src 源字符串的首地址(buf的地址)
		separator 指定的分割字符
		dest 接收子字符串的数组
		num 分割后子字符串的个数
	*/
	char* pNext;
	int count = 0;
	// 防止改变原字符串
	char* cpy = (char*)malloc_s(strlen(src) * sizeof(char*));
	memset(cpy, 0, strlen(src) * sizeof(char*));
	strcpy(cpy, src);
	if (cpy == NULL || strlen(cpy) == 0) //如果传入的地址为空或长度为0，直接终止 
		return;
	if (separator == NULL || strlen(separator) == 0) //如未指定分割的字符串，直接终止 
		return;
	pNext = (char*)strtok(cpy, separator); //必须使用(char *)进行强制类型转换
	while (pNext != NULL) {
		*dest++ = pNext;
		++count;
		pNext = (char*)strtok(NULL, separator);  //必须使用(char *)进行强制类型转换
	}
	*num = count;
}

//int value_export(char* name) {
//	struct var* itemp;
//	int	rv = 1;
//
//	if ((itemp = find_item(name, 0)) != NULL) {
//		itemp->global = 1;
//		rv = 0;
//	}
//	else if (value_store(name, (char*)"") == 1)
//		rv = value_export(name);
//	return rv;
//}

static struct var* find_item(char* name, int first_blank) {
	/*
	 * 在表中查找变量元素
	 * 找到元素，返回变量结构体的指针；否则返回NULL
	 * 但如果first_blank的值不为空，则返回第一个空白位置的指针，以便放置新变量
	 */
	int	i;
	int	len = strlen(name);
	char* s;

	for (i = 0; i < MAXVARS && table[i].str != NULL; i++) {
		s = table[i].str;
		if (strncmp(s, name, len) == 0 && s[len] == '=') {
			return &table[i];
		}
	}
	if (i < MAXVARS && first_blank)
		return &table[i];
	return NULL;
}


//void value_list() {
//	int	i;
//	for (i = 0; i < MAXVARS && table[i].str != NULL; i++)
//	{
//		if (table[i].global)
//			printf("  * %s\n", table[i].str);
//		else
//			printf("    %s\n", table[i].str);
//	}
//}
//
//int value_environ2table(char* env[]) {
//	/*
//	* 把环境变量读进来
//	*/
//	int i;
//	char* newstring;
//
//	for (i = 0; env[i] != NULL; i++) {
//		if (i == MAXVARS)
//			return 0;
//		newstring = (char*)malloc_s(1 + strlen(env[i]));
//		if (newstring == NULL)
//			return 0;
//		strcpy(newstring, env[i]);
//		table[i].str = newstring;
//		table[i].global = 1;
//	}
//	while (i < MAXVARS) {
//		table[i].str = NULL;
//		table[i++].global = 0;
//	}
//	return 1;
//}
//
//char** value_table2environ() {
//	int	i,			/* index			*/
//		j,			/* another index		*/
//		n = 0;			/* counter			*/
//	char** envtab;		/* array of pointers		*/
//
//
//	for (i = 0; i < MAXVARS && table[i].str != NULL; i++)
//		if (table[i].global == 1)
//			n++;
//	envtab = (char**)malloc_s((n + 1) * sizeof(char*));
//	if (envtab == NULL)
//		return NULL;
//
//	for (i = 0, j = 0; i < MAXVARS && table[i].str != NULL; i++)
//		if (table[i].global == 1)
//			envtab[j++] = table[i].str;
//	envtab[j] = NULL;
//	return envtab;
//}