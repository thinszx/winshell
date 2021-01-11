/*
* 作用：将某一行命令进行解析后分割，返回参数列表
* 输入：从文件中读取进来的一行shell指令
* 
* char *splitline(char *str) - 对输入进来的字符串进行解析
* char *nextline(FILE *fp) - 指向文件的下一行
*/

#include "winshell.h"
#include<stdio.h>
#include<Windows.h>

char* readline(FILE* fp) {	// 系统会自动偏移文件指针
	char* buf = (char*)malloc_s(sizeof(char));	// 读入缓冲区
	int bufsize = 0;							// 缓冲区大小
	int pos = 0;								// 当前读取到的位置
	int ch;										// 读入的字符
	int begin = 1;								// 用来去除每行开始的空白字符，当读到第一个非空字符时，将其置0
	while ((ch = getc(fp)) != EOF) {	// 当前尚未读到文件末尾
		if (begin == 1 && is_blank(ch))
			continue;
		else
			begin = 0;	// 读到第一个非空字符时，将开始空白标志置0
		if (pos + 1 >= bufsize) {		// +1是为了给\0分配空间
			if (bufsize == 0) {			// 当目前尚未有任何空间时，默认分配一个BUFSIZ，默认为512
				buf = (char*)realloc_s(buf, BUFSIZ);
			}
			else {
				buf = (char*)(realloc_s(buf, (bufsize + BUFSIZ)));
			}
			bufsize += BUFSIZ;	// 更新缓冲区大小
		}
		//如果当前的字符为'\'，则证明后面的字符需要进行转义，此时要把后面的字符也读进来
		if (ch == '\\') {
			ch = getc(fp);
			if (ch != EOF) {
				buf[pos++] = ch;
				continue;
			}
		}
		if (ch == '\n' || ch == ';') {
			break;	// 每次仅读取一行
		}
		buf[pos++] = ch;
	}
	// 当什么东西都没有读到的时候
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

#define is_delim(x) ((x) == ' '||(x)=='\t')	// 判断是否为定界符
char** splitline(char* line) {  
	char** args;		// 参数列表
	int bufsize = 0;	// 缓冲区大小
	int argsize = 0;	// 参数字符串缓冲的容量
	int argnum = 0;		// 参数个数统计
	char* cp = line;	// 指向当前要处理的字符，初始时指向line的第一个字符
	char* start;		// 指向字符数组的第一个值
	int len;			// 读取到的参数的长度，每次读取完一个参数后都会更新

	if (line == NULL) {	// 处理空行
		return NULL;
	}

	// 初始化参数列表
	args = (char**)malloc_s(BUFSIZ);
	bufsize = BUFSIZ;
	argsize = BUFSIZ / sizeof(char*);

	while (*cp != '\0') {	// 遇到定界符，指针向后移动
		while (is_delim(*cp)) {
			cp++;
		}
		if (*cp == '\0') {
			break;
		}

		// 确保参数列表足够长，最后的1是为了NULL存放而设置的
		if (argnum + 1 >= argsize) {
			args = (char**)realloc_s(args, argsize + BUFSIZ);
			bufsize += BUFSIZ;
			argsize += (BUFSIZ / sizeof(char*));
		}
		
		// 开始读取参数
		start = cp;	// 在后面会更新start为参数的第一个位置
		len = 1;
		while (*(++cp) != '\0' && !(is_delim(*cp))) {
			len++;	// 统计当前参数的长度
		}
		args[argnum++] = createstr(start, len);
	}
	args[argnum] = NULL;
	return args;
}

// 用来释放参数列表的内存
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
//	errno_t err = fopen_s(&fp, "C:\\Users\\12176\\Desktop\\操作系统课设\\winshell\\test.sh", "r");
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


// 自己实现的用来安全分配内存的代码
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