/*
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <cstring>

int main()
{
    STARTUPINFOA startup_info = { 0 };
    LPSTARTUPINFOA p_startup_info = &startup_info;
    PROCESS_INFORMATION proc_info = { 0 };
    LPPROCESS_INFORMATION p_proc_info = &proc_info;
    LPWSTR command_line =
        (LPWSTR)"C:\\Windows\\System32\\cmd.exe /C notepad.exe \"%USERPROFILE%\\Desktop\\test.txt\"";

    bool process_created = CreateProcess(
        NULL,
        command_line,
        NULL,
        NULL,
        FALSE,
        DETACHED_PROCESS,
        NULL,
        NULL,
        p_startup_info,
        p_proc_info
    );

    if (!process_created) { return -3; }

    DWORD process_exit;

    WaitForSingleObject(proc_info.hThread, INFINITE);

    GetExitCodeProcess(p_proc_info->hProcess, &process_exit);

    return (int)process_exit;
}*/

/*
void _tmain(int argc, TCHAR* argv[])
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    LPWSTR command = (LPWSTR)"dir /W";
    if (argc != 2)
    {
        printf("Usage: %s [cmdline]\n", command);
        return;
    }

    // Start the child process.
    if (!CreateProcess(NULL,   // No module name (use command line)
        command,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        TRUE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
        )
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
*/

/*
#include "pch.h"
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

//{
////different unicode strings to create notepad process
////wchar_t cmd[] = L"notepad.exe";//unicode string as parameters for strings are unicode for CreateProcessW
////wchar_t cmd[] = L"C:\\Windows\\System32\\notepad.exe";//use full path if lpApplicationName is used
//
////ping locale host forever
////wchar_t cmd[] = L"C:\\Windows\\System32\\ping.exe";
//wchar_t cmd[] = L"C:\\Windows\\System32\\cmd.exe";
////wchar_t arg[] = L" -t 127.0.0.1";
//wchar_t arg[] = L" /C dir /W";  // cmd /K，保留窗口；cmd /C，关闭窗口
//
//STARTUPINFO si = { sizeof(si) };
////memset(&si, 0, sizeof(si));//These 2 lines are the same as the init done via C style shortcut in the line above
////si.cb = sizeof(ci)
//
//PROCESS_INFORMATION pi;
//BOOL OK = CreateProcessW(cmd, arg, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);	//This is used to execute a process with arguments
////BOOL OK = CreateProcessW(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);  //运行无参数命令
//if (OK)// check if Process is created
//{
//    printf("Applicaiton is running\n");
//    printf("PID = %d\n", pi.dwProcessId);
//
//    //Wait forever till Process object is signaled (terminated)
//    DWORD status = WaitForSingleObject(pi.hProcess, INFINITE);
//
//    if (status == WAIT_OBJECT_0)//The state of the specified object is signaled.
//
//    {
//        printf("PID = %d is closed!\n", pi.dwProcessId);
//    }
//
//    CloseHandle(pi.hProcess);//Handles must be explicitly closed if not parent process will hold on to it even if child process is terminated.
//    CloseHandle(pi.hThread);
//
//
//
//}
//else
//{
//    printf("Application NOT running! \t Error code %d", GetLastError());
//}
////Add breakpoint here to prevent console from automatically closing after debugging is finished
//
//}


// 用来生成可以被winapi - CreateProcessW所接收的字符串，方便通过cmd调用
/*
char* generate_cmd(char** arglist) {
    int
    char* cmdstr = (char*)malloc_s()
}
*/

