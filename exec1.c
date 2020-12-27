#include "pch.h"
#include <stdio.h>
#include <Windows.h>

#define DEBUG

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
    wchar_t arg[] = L" /C dir /W";  // cmd /K，保留窗口；cmd /C，关闭窗口

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
    }
#endif
}


// 用来生成可以被winapi - CreateProcessW所接收的字符串，方便通过cmd调用
/*
char* generate_cmd(char** arglist) {
    int 
    char* cmdstr = (char*)malloc_s()
}
*/
