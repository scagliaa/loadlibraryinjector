#include "util.h"
#include <tlhelp32.h>
#include <winbase.h>
#include <string>

DWORD util::get_process(const char *lpProcessName)
{
    std::string current_process_name;

    PROCESSENTRY32 process_list{};

    process_list.dwSize = sizeof(process_list);
    const auto hProcList = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcList == INVALID_HANDLE_VALUE)
        return -1;

    if (!Process32First(hProcList, &process_list))
        return -1;

    current_process_name = process_list.szExeFile;

    if (current_process_name == lpProcessName)
        return process_list.th32ProcessID;

    while (Process32Next(hProcList, &process_list))
    {
        current_process_name = process_list.szExeFile;

        if (current_process_name == lpProcessName)
            return process_list.th32ProcessID;
    }
    return -1;
}