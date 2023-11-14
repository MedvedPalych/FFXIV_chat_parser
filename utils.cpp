// утилита автоопределения ID процесса винды по имени
#include "utils.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>

namespace utils {

    int find_FFXIV_PID() {
        HANDLE hSnapshot;
        PROCESSENTRY32 pe;
        int pid = 0;
        BOOL hResult;

        // snapshot of all processes in the system
        hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (INVALID_HANDLE_VALUE == hSnapshot) return 0;

        // initializing size: needed for using Process32First
        pe.dwSize = sizeof(PROCESSENTRY32);

        // info about first process encountered in a system snapshot
        hResult = Process32First(hSnapshot, &pe);

        // retrieve information about the processes
        // and exit if unsuccessful
        while (hResult) {
            // if we find the process: return process ID
            if (strcmp("ffxiv_dx11.exe", pe.szExeFile) == 0) {
                pid = pe.th32ProcessID;
                break;
            }
            hResult = Process32Next(hSnapshot, &pe);
        }

        // closes an open handle (CreateToolhelp32Snapshot)
        CloseHandle(hSnapshot);
        return pid;
    }

}