/*
* ���ã���ĳһ��������н�����ָ���ز����б�
* ���룺���ļ��ж�ȡ������һ��shellָ��
* 
* char *splitline(char *str) - ������������ַ������н���
* char *nextline(FILE *fp) - ָ���ļ�����һ��
*/

#include "winshell.h"
#include<stdio.h>
#include<Windows.h>

char* readline(FILE* fp) {	// ϵͳ���Զ�ƫ���ļ�ָ��
	char* buf = (char*)malloc_s(sizeof(char));	// ���뻺����
	int bufsize = 0;							// ��������С
	int pos = 0;								// ��ǰ��ȡ����λ��
	int ch;										// ������ַ�
	int begin = 1;								// ����ȥ��ÿ�п�ʼ�Ŀհ��ַ�����������һ���ǿ��ַ�ʱ��������0
	while ((ch = getc(fp)) != EOF) {	// ��ǰ��δ�����ļ�ĩβ
		if (begin == 1 && is_blank(ch))
			continue;
		else
			begin = 0;	// ������һ���ǿ��ַ�ʱ������ʼ�հױ�־��0
		if (pos + 1 >= bufsize) {		// +1��Ϊ�˸�\0����ռ�
			if (bufsize == 0) {			// ��Ŀǰ��δ���κοռ�ʱ��Ĭ�Ϸ���һ��BUFSIZ��Ĭ��Ϊ512
				buf = (char*)realloc_s(buf, BUFSIZ);
			}
			else {
				buf = (char*)(realloc_s(buf, (bufsize + BUFSIZ)));
			}
			bufsize += BUFSIZ;	// ���»�������С
		}
		//�����ǰ���ַ�Ϊ'\'����֤��������ַ���Ҫ����ת�壬��ʱҪ�Ѻ�����ַ�Ҳ������
		if (ch == '\\') {
			ch = getc(fp);
			if (ch != EOF) {
				buf[pos++] = ch;
				continue;
			}
		}
		if (ch == '\n' || ch == ';') {
			break;	// ÿ�ν���ȡһ��
		}
		buf[pos++] = ch;
	}
	// ��ʲô������û�ж�����ʱ��
	if (ch == EOF && pos == 0) {
		return NULL;
	}
	buf[pos] = '\0';
	/*
	char* line = (char*)malloc_s((pos) * sizeof(char));
	#pragma warning(suppress : 4996)
	strncpy(line, buf, pos);
	*/
	return buf;
}

#define is_delim(x) ((x) == ' '||(x)=='\t')	// �ж��Ƿ�Ϊ�����
char** splitline(char* line) {  
	char** args;		// �����б�
	int bufsize = 0;	// ��������С
	int argsize = 0;	// �����ַ������������
	int argnum = 0;		// ��������ͳ��
	char* cp = line;	// ָ��ǰҪ������ַ�����ʼʱָ��line�ĵ�һ���ַ�
	char* start;		// ָ���ַ�����ĵ�һ��ֵ
	int len;			// ��ȡ���Ĳ����ĳ��ȣ�ÿ�ζ�ȡ��һ�������󶼻����

	if (line == NULL) {	// �������
		return NULL;
	}

	// ��ʼ�������б�
	args = (char**)malloc_s(BUFSIZ);
	bufsize = BUFSIZ;
	argsize = BUFSIZ / sizeof(char*);

	while (*cp != '\0') {	// �����������ָ������ƶ�
		while (is_delim(*cp)) {
			cp++;
		}
		if (*cp == '\0') {
			break;
		}

		// ȷ�������б��㹻��������1��Ϊ��NULL��Ŷ����õ�
		if (argnum + 1 >= argsize) {
			args = (char**)realloc_s(args, argsize + BUFSIZ);
			bufsize += BUFSIZ;
			argsize += (BUFSIZ / sizeof(char*));
		}
		
		// ��ʼ��ȡ����
		start = cp;	// �ں�������startΪ�����ĵ�һ��λ��
		len = 1;
		while (*(++cp) != '\0' && !(is_delim(*cp))) {
			len++;	// ͳ�Ƶ�ǰ�����ĳ���
		}
		args[argnum++] = createstr(start, len);
	}
	args[argnum] = NULL;
	return args;
}

// �����ͷŲ����б���ڴ�
void freelist(char** list) {
	char** cp = list;
	while (*cp) {
		free(*cp++);
	}
	free(list);
}

//test
//int main() {
//	FILE* fp;
//	errno_t err = fopen_s(&fp, "C:\\Users\\12176\\Desktop\\����ϵͳ����\\winshell\\test.sh", "r");
//	if (fp == NULL || err != 0) {
//		printf_s("Failed to open file!");
//	}
//	//test
//	//char* buf = readline(fp);
//	//char* buf2 = readline(fp);
//	//char** args = splitline(buf);
//	int i = 0;
//	while (TRUE) {
//		char* buf = readline(fp);
//		if (buf == NULL)
//			break;
//		char** args = splitline(buf);
//		for (int i = 0; args[i] != NULL; i++) {
//			printf_s("arg[%d]%s\n", i, args[i]);
//		}
//		//printf_s("arg[%d]%s\n", i, args[i]);
//		freelist(args);
//	}/*
//	for (int i = 0; args[i] != NULL; i++) {
//		printf_s("arg[%d]%s\n", i, args[i]);
//	}*/
//	//test end
//	//printf_s(buf);
//	//printf_s(buf2);
//}
//test end

char* createstr(char* s, int l) {
	char* newstr = (char*)malloc_s(l+1);
	#pragma warning(suppress : 4996)
	strncpy(newstr, s, l);
	newstr[l] = '\0';
	return newstr;
}


// �Լ�ʵ�ֵ�������ȫ�����ڴ�Ĵ���
void* malloc_s(size_t n) {
	void* rv;
	rv = malloc(n);
	if (rv == NULL) {
		printf_s("Out of memory, failed to malloc!");
		FatalExit(1);
	}
	return rv;
}


void* realloc_s(void* p, size_t n) {
	void* rv;
	rv = realloc(p, n);
	if (rv == NULL) {
		printf_s("Failed to realloc!");
		FatalExit(1);
	}
	return rv;
}

void fatal(char* s1, char* s2, int n) {
	//fprintf_s(stderr, "Error: %s, %s\n", s1, s2);
	//exit(n);
}