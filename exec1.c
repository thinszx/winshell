//#include "pch.h"
//#include "winshell.h"
//#include <stdio.h>
//#include <Windows.h>
//
//int main()
//
///*
//BOOL CreateProcessW(
//  LPCWSTR               lpApplicationName,  // ����Ƿ�NULLֵ����Ҫʹ��lpCommandLine�Ļ���һ��Ҫдȫ·��������ֻдһ��cmd.exe
//  LPWSTR                lpCommandLine,      // ָ�����
//  LPSECURITY_ATTRIBUTES lpProcessAttributes,
//  LPSECURITY_ATTRIBUTES lpThreadAttributes,
//  BOOL                  bInheritHandles,
//  DWORD                 dwCreationFlags,
//  LPVOID                lpEnvironment,
//  LPCWSTR               lpCurrentDirectory,
//  LPSTARTUPINFOW        lpStartupInfo,
//  LPPROCESS_INFORMATION lpProcessInformation
//);
//DWORD WaitForSingleObject(
//  HANDLE hHandle,
//  DWORD  dwMilliseconds
//);
//*/
//
//{
//    wchar_t cmd[] = L"C:\\Windows\\System32\\cmd.exe";
//#ifdef DEBUG
//    // ���ļ�
//    FILE* fp;
//	// errno_t err = fopen_s(&fp, "C:\\Users\\12176\\Desktop\\����ϵͳ����\\winshell\\test.sh", "r");
//    errno_t err = fopen_s(&fp, "C:\\Users\\12176\\Desktop\\����ϵͳ����\\winshell\\iftest.sh", "r");
//	if (fp == NULL || err != 0) {
//		printf_s("Failed to open file!");
//        return -1;
//	}
//
//	int i = 0;
//    while (TRUE) {
//        char* buf = readline(fp);
//        if (buf == NULL)
//            break;
//        if (*buf == '\0')   // ���е����
//            continue;
//        char** args = splitline(buf);
//        /*
//        // �����ַ����
//        // ����ǰ������if����ʼ����������䴦�����
//        // ����if���ݲ�ִ����������һֱ����fi�ٿ�ʼִ�У���fiδ���֣����׳�����
//        if (strcmp(args[0], "if") {
//            read_if_block(fp, args);    // ����ǰ�Ѿ��еľ���Ҳ������
//            process_if_block();
//        }
//        else if(strcmp(args[0], "for"
//             || strcmp(args[0], "while")
//             || strcmp(args[0], "until"))
//        {
//            read_loop_block(fp, );
//            process_loop_block();   // �ò����ٷ�һ��while/for/untilɶ��
//        }
//        else {
//
//        }
//        // check_args();    // ��鵱ǰ�������Ƿ�Ϊif����������
//        process_args()
//        */
//        process(args);
//        /* �����������һ������process.c�з�װ���˺����������������
//        wchar_t* arg = generate_cmd(buf);
//        STARTUPINFO si = { sizeof(si) };
//
//        PROCESS_INFORMATION pi;
//        BOOL OK = CreateProcessW(cmd, arg, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);	// �����в�������
//        //BOOL OK = CreateProcessW(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);  // �����޲����������ֻ��Ϊ�����ʼǣ���ʵ�����������ò���
//        if (OK) { // ���̴����ɹ�
//            printf("Applicaiton is running\n");
//            printf("PID = %d\n", pi.dwProcessId);
//
//            // �ȴ����̽������ȴ�ʱ��Ϊ����
//            DWORD status = WaitForSingleObject(pi.hProcess, INFINITE);
//
//            // �����̽�����״̬��
//            DWORD dwErr;
//            GetExitCodeProcess(pi.hProcess, &dwErr);
//            if (dwErr)
//                printf("Failed to execute command, exit code %d\n", dwErr);
//            // printf_s("Exit code: %d\n", dwErr); // ����ʱ���Ϊ1
//
//            if (status == WAIT_OBJECT_0) {
//                printf("PID = %d is closed!\n\n", pi.dwProcessId);
//            }
//            free(buf);
//            free(arg);
//            CloseHandle(pi.hProcess);
//            CloseHandle(pi.hThread);
//        }
//        else {
//            printf("Application NOT running! \t Error code %d", GetLastError());
//        }*/
//        /*
//		char** args = splitline(buf);
//		for (int i = 0; args[i] != NULL; i++) {
//			printf_s("arg[%d]%s\n", i, args[i]);
//		}
//		//printf_s("arg[%d]%s\n", i, args[i]);
//		freelist(args);*/
//	}/* �ⲿ�ֱ�����û��
//	for (int i = 0; args[i] != NULL; i++) {
//		printf_s("arg[%d]%s\n", i, args[i]);
//	}*/
//	//test end
//	//printf_s(buf);
//	//printf_s(buf2);
//    //wchar_t arg[] = L" /C dir /W";  // cmd /K���������ڣ�cmd /C���رմ���
//    //wchar_t arg[] = generate_cmd(args);
//    /*
//    STARTUPINFO si = { sizeof(si) };
//    
//    PROCESS_INFORMATION pi;
//    BOOL OK = CreateProcessW(cmd, arg, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);	// �����в�������
//    //BOOL OK = CreateProcessW(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);  // �����޲����������ֻ��Ϊ�����ʼǣ���ʵ�����������ò���
//    if (OK) { // ���̴����ɹ�
//        printf("Applicaiton is running\n");
//        printf("PID = %d\n", pi.dwProcessId);
//
//        // �ȴ����̽������ȴ�ʱ��Ϊ����
//        DWORD status = WaitForSingleObject(pi.hProcess, INFINITE);
//
//        if (status == WAIT_OBJECT_0) {
//            printf("PID = %d is closed!\n", pi.dwProcessId);
//        }
//
//        CloseHandle(pi.hProcess);
//        CloseHandle(pi.hThread);
//    }
//    else {
//        printf("Application NOT running! \t Error code %d", GetLastError());
//    }*/
//    #endif
//}
//
//wchar_t* char2wchar(char* text, size_t* converted) {
//    const size_t size = strlen(text) + 1;
//    wchar_t* wtext = (wchar_t*)malloc_s(strlen(text)*sizeof(wchar_t));
//    mbstowcs_s(converted, wtext, size, text, _TRUNCATE);
//    return wtext;
//}
//
//// �������ɿ��Ա�winapi - CreateProcessW�����յ��ַ���������ͨ��cmd����
//wchar_t* generate_cmd(char* str) {
//    size_t converted = 0;
//    wchar_t* cmdstr = char2wchar(str, &converted);
//    wchar_t* cmdall = (wchar_t*)malloc_s((3 + converted) * sizeof(wchar_t));
//    memset(cmdall, 0, (3 + converted) * sizeof(wchar_t));
//    errno_t err = 0;
//    //wchar_t* cmdall = L"/C ";
//    err = wcscat_s(cmdall, 3 + converted, L"/C ");
//    err = wcscat_s(cmdall, 3 + converted, cmdstr);
//    if (err != 0) {
//        printf_s("Failed to generate command line!");
//        return NULL;
//    }
//    return cmdall;
//}
//
//
