/*
* ����ִ��
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


/* ִ���������ֵΪִ�н��
* 1. ������Ҫ�������char**����ת��Ϊƴ�ӳ�һ����char*�ַ���
*	 {"echo", "this"} -> "echo this" ----- combine_char()
* 2. ��char*ת��Ϊwchar_t*��������һ���õ����ַ���ǰ����L"/C "
*	 /C ��cmd��һ������ -------------------- generate_cmd()
* 3. ����cmdִ�������������̵���ʼ��������ʧ��ʱ��ʾ�˳����̵�exit code����
*/
DWORD execute(char** args) {
	char* buf = concatenate_char_array(args);	// �洢argsƴ���������ַ���
	// printf_s("%s\n", buf);

	wchar_t cmd[] = L"C:\\Windows\\System32\\cmd.exe";
	wchar_t* arg = generate_cmd(buf);
	STARTUPINFO si = { sizeof(si) };

	PROCESS_INFORMATION pi;
	BOOL OK = CreateProcessW(cmd, arg, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);	// �����в�������
	//BOOL OK = CreateProcessW(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);  // �����޲����������ֻ��Ϊ�����ʼǣ���ʵ�����������ò���
	// �����̽�����״̬��
	DWORD dwErr = 0;
	if (OK) { // ���̴����ɹ�
		printf("Applicaiton is running\n");
		printf("PID = %d\n", pi.dwProcessId);

		// �ȴ����̽������ȴ�ʱ��Ϊ����
		DWORD status = WaitForSingleObject(pi.hProcess, INFINITE);

		GetExitCodeProcess(pi.hProcess, &dwErr);
		if (dwErr)
			printf("Failed to execute command, exit code %d\n", dwErr);
		// printf_s("Exit code: %d\n", dwErr); // ����ʱ���Ϊ1

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

// ���Ӷ���ַ���
// ����Ԥ�Ȳ�֪���ַ������ȣ�����û����strcat����������strcpy+����س��ȷ���������ƴ��
// strcatҪ��Ի��������г�ʼ������ƴ�ӵ�һ��ʱ��������ʱ��ֵ�ģ������ֻ��ʼ��δ����ĵط���ܺ�ʱ��
// ���Ըɴ�Ԥ���建��ش�С���������ٷ�����ÿ�η���ֱ�Ӱ�֮ǰ�Ļ�����strcpy���»������У�Ȼ����free֮ǰ������
/*
char* concatenate_char_array(char** arr) {
	char** posarray = arr;
	char* buf = (char*)malloc_s(sizeof(char));
	int bufsize = 0;
	int arraysize = 0;	// ��ǰָ���char*�Ĵ�С
	int pos = 0;
	while (posarray != NULL) {
		arraysize = strlen(*posarray) * sizeof(char);
		if (pos + arraysize >= bufsize) {		// ����+1��\0����ռ䣬��Ϊsizeof����\0
			if (bufsize == 0) {			// ��Ŀǰ��δ���κοռ�ʱ��Ĭ�Ϸ���һ��BUFSIZ��Ĭ��Ϊ512
				buf = (char*)realloc_s(buf, BUFSIZ);
				// memset(buf, 0, sizeof(buf));	// ��ʼ�����������������strcat_s�ᱨ��
			}
			else {
				buf = (char*)(realloc_s(buf, (bufsize + BUFSIZ)));
			}
			bufsize += BUFSIZ;	// ���»�������С
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
	int arraysize = 0;	// ��ǰָ���char*�Ĵ�С
	int pos = 0;		// �����Ѿ����˶��ٻ������ڴ�
	while (*posarray != NULL) {
		char* arg = *posarray;
		// �ڷǵ�һ������֮ǰ��һ���ո�
		if (pos == 0)	// ��һ��������ǰ�治�üӿո�
			arraysize = strlen(arg) * sizeof(char);
		else {
			arraysize = strlen(arg) * sizeof(char) + 2;	// ����+1��Ϊ�˸����ǰ�Ŀո����ռ�
			char* tmp = (char*)malloc_s(arraysize);
			sprintf_s(tmp, arraysize, "%c%s", ' ', arg);
			arg = tmp;
		}
		
		if (pos + arraysize >= bufsize) {		// ����+1��\0����ռ䣬��Ϊsizeof����\0
			if (bufsize == 0) {			// ��Ŀǰ��δ���κοռ�ʱ��Ĭ�Ϸ���һ��BUFSIZ��Ĭ��Ϊ512
				buf = (char*)realloc_s(buf, BUFSIZ);
				memset(buf, 0, BUFSIZ);	// ��ʼ�����������������strcat_s�ᱨ��
			}
			else {
				char* newbuf = (char*)(malloc_s(bufsize + BUFSIZ));
				memset(newbuf, 0, bufsize + BUFSIZ);
				// memmove_s(newbuf, bufsize + BUFSIZ, buf, bufsize);
				strcpy_s(newbuf, bufsize, buf);
				free(buf);	// �ͷ�֮ǰ���ڴ�
				buf = newbuf;
			}
			bufsize += BUFSIZ;	// ���»�������С
		}

		pos += arraysize;
		strcat_s(buf, bufsize, arg);
		posarray++;
	}
	return buf;
}