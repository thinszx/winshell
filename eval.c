#define _CRT_SECURE_NO_WARNINGS
/* ����ժҪ
 *		����ִ�и�ֵ��䣬�ڶ���һ�����ȥ�����п��ܴ��ڵ�if/while�ȹؼ��ʺ�����ִ�еľ��Ǳ��ļ�������
 * ����˵��
 *		����ֵ��ӳ�˸�����Ƿ�Ϊ�����ĸ�ֵ��䣬����ֵΪ0�����ǣ�Ϊ-1������
 * �����߼�
 *		�ڽ��и�ֵʱ����ά��һ��hash�������������Լ����ǵ�ֵ��Ϊ��ֵ�Է���hash����
 *		���ݷ���ֵ
 */
#include <stdio.h>
#include <ctype.h>
#include "varlib.h"
#include <Windows.h>

// ��$abc������ʽ���ַ���ת��Ϊ$abc��ֵ
int eval(char* src, char* dest, int len) {
	int	pos;			// destָ��
	char ch;			// ��ǰ�ַ�
	char* varval;		// ָ�����

	pos = 0;
	while (ch = *src++) {
		if (pos == len)			// Խ����
			return -1;
		if (ch == '\0' || ch == EOF) {
			break;
		}
		// ����������Ҫ������������splitline.c�����Ѿ�����ת���ˣ������$һ���Ǳ�����ͷ�ģ�\$һ����ת��
		if (ch == '\\')
			if (*src == '$')	// ��\����ת��
			{
				dest[pos++] = *src;
				src++;
				continue;
			}

		if (ch != '$')			// ��Ϊ�����ַ�
		{
			dest[pos++] = ch;	// д�뻺����
			continue;
		}

		/* ��$����� */
		varval = getvar(&src);	// ���Զ�ȡ������
		if (strlen(varval) + pos >= len)	// Խ����
			return -1;

		strcpy(dest + pos, varval);		// д�����
		pos += strlen(varval);			// �ƶ�ָ��
	}
	dest[pos] = '\0';
	return pos;
}

char* getvar(char** srcp) {
/*
 * ���Զ�ȡ������ȫ�ƣ��ڹ����м��Ϸ���
 * �������ƺϷ������ر�������
 * ���򷵻�$��������*srcp����
 */
	char varname[VARLEN + 1];	// ��������
	int	pos;					// д�뻺������ָ��λ��
	char* cp;					// �������ɱ�����ָ��

	cp = *srcp;				
	pos = 0;

	while (is_an_idchar(*cp, pos))	// ��ǰ�ַ��Ϸ�
		varname[pos++] = *cp++;	
	varname[pos] = '\0';
	*srcp = cp;

	if (pos > 0) {	// �����ַ��Ϸ�
		// cp = EVget(varname);
		cp = value_lookup(varname);
		//if (cp == NULL)		// �����Ҳ���ʱ����ֵ����""
		//	cp = (char*)"";
	}
	else
		cp = (char*)"$";			/* else return the $ */
	return cp;
}


int isaname(char* s) {
	/*
	 *	�ж���ǰ�ַ����Ƿ��ǺϷ�������
	 */
	int	pos = 0;
	while (*s && is_an_idchar(*s, pos++))
		s++;

	return ((!*s) && (pos != 0));
}

int is_an_idchar(char ch, int pos) {
	/*
	 *	�ж���ǰ�ַ��Ƿ����λ�ںϷ���������
	 */
	return (isalpha(ch) || ch == '_' || (isdigit(ch) && pos > 0));
}
