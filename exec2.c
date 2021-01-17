#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "winshell2.h"
#include "varlib.h"
#include <stdio.h>
#include <Windows.h>

int main()

/*
BOOL CreateProcessW(
  LPCWSTR               lpApplicationName,  // 如果是非NULL值并且要使用lpCommandLine的话，一定要写全路径，不能只写一个cmd.exe
  LPWSTR                lpCommandLine,      // 指令参数
  LPSECURITY_ATTRIBUTES lpProcessAttributes,
  LPSECURITY_ATTRIBUTES lpThreadAttributes,
  BOOL                  bInheritHandles,
  DWORD                 dwCreationFlags,
  LPVOID                lpEnvironment,
  LPCWSTR               lpCurrentDirectory,
  LPSTARTUPINFOW        lpStartupInfo,
  LPPROCESS_INFORMATION lpProcessInformation
);
DWORD WaitForSingleObject(
  HANDLE hHandle,
  DWORD  dwMilliseconds
);
*/

{
    wchar_t cmd[] = L"C:\\Windows\\System32\\cmd.exe";
#ifdef DEBUG
    // 打开文件
    FILE* fp;
    // errno_t err = fopen_s(&fp, "C:\\Users\\12176\\Desktop\\操作系统课设\\winshell\\test.sh", "r");
    // errno_t err = fopen_s(&fp, "C:\\Users\\12176\\Desktop\\操作系统课设\\winshell\\iftest.sh", "r");
    // errno_t err = fopen_s(&fp, "C:\\Users\\12176\\Desktop\\操作系统课设\\winshell\\multiif.sh", "r");
    // errno_t err = fopen_s(&fp, "C:\\Users\\12176\\Desktop\\操作系统课设\\winshell\\fail-if.sh", "r");
    // errno_t err = fopen_s(&fp, "C:\\Users\\12176\\Desktop\\操作系统课设\\winshell\\lessfi.sh", "r");
    // errno_t err = fopen_s(&fp, "C:\\Users\\12176\\Desktop\\操作系统课设\\winshell\\plusfi.sh", "r");
    // errno_t err = fopen_s(&fp, "C:\\Users\\12176\\Desktop\\操作系统课设\\winshell\\var_assign.sh", "r");
    errno_t err = fopen_s(&fp, "C:\\Users\\12176\\Desktop\\操作系统课设\\winshell\\loop.sh", "r");
    if (fp == NULL || err != 0) {
        printf_s("Failed to open file!");
        return -1;
    }

    int i = 0;
    while (TRUE) {
        // char newstr[MAXCMDLEN];
        char* buf = readline(fp);
        if (buf == NULL) {
            if_check(buf);
            break;
        }
        // 尝试进行变量的赋值
        // 防止echo a=$a这样的代码出现，如果等号前有空格，那么一定不是赋值语句
        if ((strncmp(buf, "if ", 3) != 0) && (strncmp(buf, "else ", 5) != 0) &&
            (strncmp(buf, "while ", 6) != 0) && (strncmp(buf, "elif ", 5) != 0) && (check_inside(buf) != 1)) {
            int assigned = value_assign(buf);
            if (assigned == 0)
                continue;
        }

        //if (eval(buf, newstr, MAXCMDLEN) == -1)
        //{
        //    fprintf(stderr, "line too long\n");
        //    return 1;
        //}
        //printf("%s", newstr);

        if (*buf == '\0')   // 空行的情况
            continue;
        char newstr[MAXCMDLEN];
        // while
        if (strncmp(buf, "while ", 6) == 0) {
            char** tmplist = splitline(buf);
            strcpy(buf, buf + 6);
            set_loop_condition(buf);
            char** lines = read_loop_block(fp);
            int ret = execute_loop(lines);
            if (ret == 1)
                continue;
        }

        if (eval(buf, newstr, MAXCMDLEN) == -1) {
            fprintf(stderr, "line too long\n");
            return 1;
        }
        // printf("%s", newstr);
        char** args = splitline(newstr);
        if (if_check(newstr) == OK_EXEC)
            process(newstr);   // 执行语句
        

        /*
        // 特殊字符检查
        // 若当前读到了if，开始进入条件语句处理程序
        // 读到if后暂不执行命令，将向后一直读到fi再开始执行，若fi未出现，则抛出错误，
        if (strcmp(args[0], "if") {
            read_if_block(fp, args);    // 将当前已经有的句子也读进来
            process_if_block();
        }
        else if(strcmp(args[0], "for"
             || strcmp(args[0], "while")
             || strcmp(args[0], "until"))
        {
            read_loop_block(fp, );
            process_loop_block();   // 用不用再分一下while/for/until啥的
        }
        else {

        }
        // check_args();    // 检查当前的命令是否为if等特殊命令
        process_args()
        */
        // process(args);
        /* 方便起见，这一部分在process.c中封装成了函数供其他程序调用
        wchar_t* arg = generate_cmd(buf);
        STARTUPINFO si = { sizeof(si) };

        PROCESS_INFORMATION pi;
        BOOL OK = CreateProcessW(cmd, arg, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);	// 运行有参数命令
        //BOOL OK = CreateProcessW(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);  // 运行无参数命令，这行只是为了做笔记，其实这个程序根本用不到
        if (OK) { // 进程创建成功
            printf("Applicaiton is running\n");
            printf("PID = %d\n", pi.dwProcessId);

            // 等待进程结束，等待时间为永久
            DWORD status = WaitForSingleObject(pi.hProcess, INFINITE);

            // 检查进程结束的状态码
            DWORD dwErr;
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
        }*/
        /*
        char** args = splitline(buf);
        for (int i = 0; args[i] != NULL; i++) {
            printf_s("arg[%d]%s\n", i, args[i]);
        }
        //printf_s("arg[%d]%s\n", i, args[i]);
        freelist(args);*/
    }
    // if_check(NULL);
    /* 这部分本来就没用
    for (int i = 0; args[i] != NULL; i++) {
        printf_s("arg[%d]%s\n", i, args[i]);
    }*/
    //test end
    //printf_s(buf);
    //printf_s(buf2);
    //wchar_t arg[] = L" /C dir /W";  // cmd /K，保留窗口；cmd /C，关闭窗口
    //wchar_t arg[] = generate_cmd(args);
    /*
    STARTUPINFO si = { sizeof(si) };

    PROCESS_INFORMATION pi;
    BOOL OK = CreateProcessW(cmd, arg, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);	// 运行有参数命令
    //BOOL OK = CreateProcessW(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);  // 运行无参数命令，这行只是为了做笔记，其实这个程序根本用不到
    if (OK) { // 进程创建成功
        printf("Applicaiton is running\n");
        printf("PID = %d\n", pi.dwProcessId);

        // 等待进程结束，等待时间为永久
        DWORD status = WaitForSingleObject(pi.hProcess, INFINITE);

        if (status == WAIT_OBJECT_0) {
            printf("PID = %d is closed!\n", pi.dwProcessId);
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        printf("Application NOT running! \t Error code %d", GetLastError());
    }*/
#endif
}

wchar_t* char2wchar(char* text, size_t* converted) {
    const size_t size = strlen(text) + 1;
    wchar_t* wtext = (wchar_t*)malloc_s(strlen(text) * sizeof(wchar_t));
    mbstowcs_s(converted, wtext, size, text, _TRUNCATE);
    return wtext;
}

// 用来生成可以被winapi - CreateProcessW所接收的字符串，方便通过cmd调用
wchar_t* generate_cmd(char* str) {
    size_t converted = 0;
    wchar_t* cmdstr = char2wchar(str, &converted);
    wchar_t* cmdall = (wchar_t*)malloc_s((3 + converted) * sizeof(wchar_t));
    memset(cmdall, 0, (3 + converted) * sizeof(wchar_t));
    errno_t err = 0;
    //wchar_t* cmdall = L"/C ";
    err = wcscat_s(cmdall, 3 + converted, L"/C ");
    err = wcscat_s(cmdall, 3 + converted, cmdstr);
    if (err != 0) {
        printf_s("Failed to generate command line!");
        return NULL;
    }
    return cmdall;
}


