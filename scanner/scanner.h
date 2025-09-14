#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#pragma comment(lib, "ntdll.lib")

typedef enum _MEMORY_INFORMATION_CLASS
{
    MemoryBasicInformation
} MEMORY_INFORMATION_CLASS;

#if defined(_WIN64)
extern "C" NTSYSCALLAPI NTSTATUS ZwReadVirtualMemory(
    HANDLE  hProcess,
    LPCVOID lpBaseAddress,
    LPVOID  lpBuffer,
    SIZE_T  nSize,
    SIZE_T* lpNumberOfBytesRead
);

extern "C" NTSYSCALLAPI NTSTATUS ZwWriteVirtualMemory(
    HANDLE  hProcess,
    LPVOID  lpBaseAddress,
    LPCVOID lpBuffer,
    SIZE_T  nSize,
    SIZE_T* lpNumberOfBytesWritten
);

extern "C" NTSYSCALLAPI NTSTATUS NtQueryVirtualMemory(
    HANDLE                   ProcessHandle,
    PVOID                    BaseAddress,
    MEMORY_INFORMATION_CLASS MemoryInformationClass,
    PVOID                    MemoryInformation,
    SIZE_T                   MemoryInformationLength,
    PSIZE_T                  ReturnLength
);
#elif defined(_WIN32)
extern "C" NTSTATUS NTAPI NtQueryVirtualMemory(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    MEMORY_INFORMATION_CLASS MemoryInformationClass,
    PVOID MemoryInformation,
    SIZE_T MemoryInformationLength,
    PSIZE_T ReturnLength);

extern "C" NTSTATUS NTAPI ZwReadVirtualMemory(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    PVOID Buffer,
    SIZE_T BufferSize,
    PSIZE_T NumberOfBytesRead);

extern "C" NTSTATUS NTAPI ZwWriteVirtualMemory(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    PVOID Buffer,
    SIZE_T BufferSize,
    PSIZE_T NumberOfBytesWritten);
#endif


static bool flag;

class scanner
{
public:
    scanner(DWORD proccesid);
    ~scanner();
    void scanstring(std::string stringtofind);
    std::vector<uintptr_t> returnaddreses();
private:
    std::vector<uintptr_t> addres;
    MEMORY_BASIC_INFORMATION info;
    ;
    HANDLE hProcess;
    SYSTEM_INFO si;
    char* currentmemorypage = 0;
};
#pragma once
