#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include "varlib.h"
#include "winshell2.h"

#define	MAXVARS	200	// �洢�������������

struct var {
	char* str;		// str�ĸ�ʽΪa=2
	int  global;	// �Ƿ�Ϊȫ�ֱ�������ʱû��
};

static struct var table[MAXVARS];	// �洢������table

static char* new_string(char*, char*);
static struct var* find_item(char*, int);

int value_store(char* name, char* val) {
	/*
	 * �����б�����ҵ������û��ñ���ֵ�������򽫱�����ӵ�ĩβ
	 * ��֧��ɾ������������@TODO
	 * ����ֵΪ1������Ϊ��������ֵΪ0������Ϊ����ִ��
	 */
	struct var* itemp;
	char* s;
	int	rv = 1;

	// ���ұ���������Ϊ������ֵ
	if ((itemp = find_item(name, 1)) != NULL && (s = new_string(name, val)) != NULL)
	{
		if (itemp->str)			// ��������
			free(itemp->str);	// ɾ���ñ�����ֵ
		itemp->str = s;			// ����ʱ����Ϊ������ֵ��������ʱ��������ֵ
		rv = 0;
	}
	return rv;
}

char* new_string(char* name, char* val) {
	/*
	 * ����һ��name=value��ʽ���ַ���
	 */
	char* retval;

	retval = (char*)malloc_s(strlen(name) + strlen(val) + 2);
	if (retval != NULL)
		sprintf_s(retval, strlen(name) + strlen(val) + 2, "%s=%s", name, val);
	return retval;
}

char* value_lookup(char* name) {
	/*
	 * ���鵽�������򷵻ر�����ֵ
	 * ���򷵻ؿ��ַ���
	 */
	struct var* itemp;

	if ((itemp = find_item(name, 0)) != NULL)
		return itemp->str + 1 + strlen(name);	// ָ���Ƶ��������͵Ⱥź�
	return (char*)"";

}

int value_assign(char* cmd) {
	/* ���Խ��б�����ֵ�����Ҳ����Ⱥţ���֤�����Ǳ�����ֵ��䣬����-1 */
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
		if (strstr((const char*)dest[1], (const char*)"+") != NULL) {	// ֻʵ���˼ӷ���û�м��ӷ��ĺϷ��ԣ���Ȼ������̫�Ӵ���...
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
		src Դ�ַ������׵�ַ(buf�ĵ�ַ)
		separator ָ���ķָ��ַ�
		dest �������ַ���������
		num �ָ�����ַ����ĸ���
	*/
	char* pNext;
	int count = 0;
	// ��ֹ�ı�ԭ�ַ���
	char* cpy = (char*)malloc_s(strlen(src) * sizeof(char*));
	memset(cpy, 0, strlen(src) * sizeof(char*));
	strcpy(cpy, src);
	if (cpy == NULL || strlen(cpy) == 0) //�������ĵ�ַΪ�ջ򳤶�Ϊ0��ֱ����ֹ 
		return;
	if (separator == NULL || strlen(separator) == 0) //��δָ���ָ���ַ�����ֱ����ֹ 
		return;
	pNext = (char*)strtok(cpy, separator); //����ʹ��(char *)����ǿ������ת��
	while (pNext != NULL) {
		*dest++ = pNext;
		++count;
		pNext = (char*)strtok(NULL, separator);  //����ʹ��(char *)����ǿ������ת��
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
	 * �ڱ��в��ұ���Ԫ��
	 * �ҵ�Ԫ�أ����ر����ṹ���ָ�룻���򷵻�NULL
	 * �����first_blank��ֵ��Ϊ�գ��򷵻ص�һ���հ�λ�õ�ָ�룬�Ա�����±���
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
//	* �ѻ�������������
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