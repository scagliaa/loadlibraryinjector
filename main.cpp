#include "util.h"
#include <iostream>
#include <string>
#include <tlhelp32.h>
#include <windows.h>

int main(int argument_count, char* argument_values[]) {
    std::string dll_name;
    std::string process_name;
    char full_dll_path[MAX_PATH];

    if (argument_count == 3) {
        dll_name = argument_values[1];
        process_name = argument_values[2];
    } else {
        std::cout << "inject.exe <dll> <process>\n";
        return -1;
    }

    DWORD process_id = util::get_process(process_name.c_str());
    if (process_id == static_cast<DWORD>(-1)) {
        std::cout << "An error occurred when trying to find the target process.\n";
        return -1;
    }

    std::cout << "process : " << process_name << "\n";
    std::cout << "proc id : " << static_cast<int>(process_id) << "\n\n";

    DWORD full_path_result = GetFullPathNameA(dll_name.c_str(), MAX_PATH, full_dll_path, nullptr);

    if (full_path_result == 0) {
        HANDLE target_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
        if (target_process == INVALID_HANDLE_VALUE) {
            return -1;
        }

        std::cout << "Process opened successfully.\n";

        LPVOID path_address = VirtualAllocEx(target_process, nullptr, strlen(full_dll_path) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (path_address == nullptr) return -1;

        std::cout << "allocation  0x" << std::hex << reinterpret_cast<uintptr_t>(path_address) << "\n";

        DWORD write_result = WriteProcessMemory(target_process, path_address, full_dll_path, strlen(full_dll_path) + 1, nullptr);
        if (write_result == 0) return -1;

        std::cout << "memory has been written successfully";

        HMODULE module = GetModuleHandleA("kernel32.dll");
        if (module == INVALID_HANDLE_VALUE || module == nullptr)
            return -1;

        FARPROC function_address = GetProcAddress(module, "LoadLibraryA");
        if (function_address == nullptr)
            return -1;

        std::cout << "LoadLibraryA is at 0x" << std::hex << reinterpret_cast<uintptr_t>(function_address) << "\n";

        HANDLE thread_creation_result = CreateRemoteThread(target_process, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(function_address), path_address, 0, nullptr);
        if (thread_creation_result == INVALID_HANDLE_VALUE) return -1;

        std::cout << "Thread created and handled the value, dll injected. !\n";

        return 0;
    }
}