#include "pch.h"
#include <stdio.h>
#include <Windows.h>

#define DEBUG

int main()

/*
BOOL CreateProcessW(
  LPCWSTR               lpApplicationName,  // ����Ƿ�NULLֵ����Ҫʹ��lpCommandLine�Ļ���һ��Ҫдȫ·��������ֻдһ��cmd.exe
  LPWSTR                lpCommandLine,      // ָ�����
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
    wchar_t arg[] = L" /C dir /W";  // cmd /K���������ڣ�cmd /C���رմ���

    STARTUPINFO si = { sizeof(si) };

    PROCESS_INFORMATION pi;
    BOOL OK = CreateProcessW(cmd, arg, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);	// �����в�������
    //BOOL OK = CreateProcessW(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);  // �����޲����������ֻ��Ϊ�����ʼǣ���ʵ�����������ò���
    if (OK) { // ���̴����ɹ�
        printf("Applicaiton is running\n");
        printf("PID = %d\n", pi.dwProcessId);

        // �ȴ����̽������ȴ�ʱ��Ϊ����
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


// �������ɿ��Ա�winapi - CreateProcessW�����յ��ַ���������ͨ��cmd����
/*
char* generate_cmd(char** arglist) {
    int 
    char* cmdstr = (char*)malloc_s()
}
*/
