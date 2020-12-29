/*
* 进程执行
*/
#include <Windows.h>
#include "winshell.h"

DWORD process(char** args) {
	int rv = 0;
	if (args[0] == NULL)
		rv = 0;
	else if (is_control_command(args[0]))
		rv = do_control_command(args);
	else if (execute_check() == 0)
		rv = execute(args);
	return rv;
}


/* 执行命令，返回值为执行结果
* 1. 首先需要将命令的char**数组转换为拼接成一整个char*字符串
*	 {"echo", "this"} -> "echo this" ----- combine_char()
* 2. 将char*转换为wchar_t*，并在上一步得到的字符串前加上L"/C "
*	 /C 是cmd的一个参数 -------------------- generate_cmd()
* 3. 调用cmd执行命令，并输出进程的起始、结束，失败时提示退出进程的exit code错误
*/
DWORD execute(char** args) {
	char* buf = concatenate_char_array(args);	// 存储args拼接起来的字符串
	// printf_s("%s\n", buf);

	wchar_t cmd[] = L"C:\\Windows\\System32\\cmd.exe";
	wchar_t* arg = generate_cmd(buf);
	STARTUPINFO si = { sizeof(si) };

	PROCESS_INFORMATION pi;
	BOOL OK = CreateProcessW(cmd, arg, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);	// 运行有参数命令
	//BOOL OK = CreateProcessW(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);  // 运行无参数命令，这行只是为了做笔记，其实这个程序根本用不到
	// 检查进程结束的状态码
	DWORD dwErr = 0;
	if (OK) { // 进程创建成功
		printf("Applicaiton is running\n");
		printf("PID = %d\n", pi.dwProcessId);

		// 等待进程结束，等待时间为永久
		DWORD status = WaitForSingleObject(pi.hProcess, INFINITE);

		GetExitCodeProcess(pi.hProcess, &dwErr);
		if (dwErr)
			printf("Failed to execute command, exit code %d\n", dwErr);
		// printf_s("Exit code: %d\n", dwErr); // 错误时输出为1

		if (status == WAIT_OBJECT_0) {
			printf("PID = %d is closed!\n\n", pi.dwProcessId);
		}
		free(buf);
		free(arg);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else {
		printf("Application NOT running! \t Error code %d", GetLastError());
	}
		/*
		char** args = splitline(buf);
		for (int i = 0; args[i] != NULL; i++) {
			printf_s("arg[%d]%s\n", i, args[i]);
		}
		//printf_s("arg[%d]%s\n", i, args[i]);
		freelist(args);*/

	return dwErr;
}

// 连接多个字符串
// 由于预先不知道字符串长度，这里没有用strcat，而是用了strcpy+缓冲池长度翻倍进行了拼接
// strcat要求对缓冲区进行初始化，但拼接到一半时，缓冲区时有值的，而如果只初始化未分配的地方会很耗时间
// 所以干脆预定义缓冲池大小，不够了再翻倍，每次翻倍直接把之前的缓冲区strcpy到新缓冲区中，然后再free之前缓冲区
/*
char* concatenate_char_array(char** arr) {
	char** posarray = arr;
	char* buf = (char*)malloc_s(sizeof(char));
	int bufsize = 0;
	int arraysize = 0;	// 当前指向的char*的大小
	int pos = 0;
	while (posarray != NULL) {
		arraysize = strlen(*posarray) * sizeof(char);
		if (pos + arraysize >= bufsize) {		// 不用+1给\0分配空间，因为sizeof包含\0
			if (bufsize == 0) {			// 当目前尚未有任何空间时，默认分配一个BUFSIZ，默认为512
				buf = (char*)realloc_s(buf, BUFSIZ);
				// memset(buf, 0, sizeof(buf));	// 初始化缓冲区，否则调用strcat_s会报错
			}
			else {
				buf = (char*)(realloc_s(buf, (bufsize + BUFSIZ)));
			}
			bufsize += BUFSIZ;	// 更新缓冲区大小
		}
#pragma warning(suppress : 4996)
		strncat_s(buf, *posarray);
		posarray++;
	}
	return buf;
}
*/

char* concatenate_char_array(char** arr) {
	char** posarray = arr;
	char* buf = (char*)malloc_s(sizeof(char));
	int bufsize = 0;
	int arraysize = 0;	// 当前指向的char*的大小
	int pos = 0;		// 现在已经用了多少缓冲区内存
	while (*posarray != NULL) {
		char* arg = *posarray;
		// 在非第一个参数之前加一个空格
		if (pos == 0)	// 第一个参数，前面不用加空格
			arraysize = strlen(arg) * sizeof(char);
		else {
			arraysize = strlen(arg) * sizeof(char) + 2;	// 这里+1是为了给语句前的空格分配空间
			char* tmp = (char*)malloc_s(arraysize);
			sprintf_s(tmp, arraysize, "%c%s", ' ', arg);
			arg = tmp;
		}
		
		if (pos + arraysize >= bufsize) {		// 不用+1给\0分配空间，因为sizeof包含\0
			if (bufsize == 0) {			// 当目前尚未有任何空间时，默认分配一个BUFSIZ，默认为512
				buf = (char*)realloc_s(buf, BUFSIZ);
				memset(buf, 0, BUFSIZ);	// 初始化缓冲区，否则调用strcat_s会报错
			}
			else {
				char* newbuf = (char*)(malloc_s(bufsize + BUFSIZ));
				memset(newbuf, 0, bufsize + BUFSIZ);
				// memmove_s(newbuf, bufsize + BUFSIZ, buf, bufsize);
				strcpy_s(newbuf, bufsize, buf);
				free(buf);	// 释放之前的内存
				buf = newbuf;
			}
			bufsize += BUFSIZ;	// 更新缓冲区大小
		}

		pos += arraysize;
		strcat_s(buf, bufsize, arg);
		posarray++;
	}
	return buf;
}