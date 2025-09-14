
#include "scanner.h"


scanner::scanner(DWORD proccesid)
{
    GetSystemInfo(&si);
   // flag = true;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, proccesid);
}

scanner::~scanner()
{
    CloseHandle(hProcess);
}
void scanner::scanstring(std::string stringtofind)
{
    const size_t stringLength = stringtofind.size();
    const size_t chunkSize = 4096;
    MEMORY_BASIC_INFORMATION info;
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    while (flag && currentmemorypage < si.lpMaximumApplicationAddress) {
        if (NtQueryVirtualMemory(hProcess, currentmemorypage, MemoryBasicInformation, &info, sizeof(info), nullptr) != 0) {
            break;
        }
        if (info.State == MEM_COMMIT && info.Protect == PAGE_READWRITE) {
            uintptr_t regionStart = reinterpret_cast<uintptr_t>(currentmemorypage);
            uintptr_t regionEnd = regionStart + info.RegionSize;

            while (regionStart < regionEnd && flag) {
                uintptr_t bytesToRead = (regionEnd - regionStart < chunkSize) ? (regionEnd - regionStart) : chunkSize;
                std::vector<char> buffer(bytesToRead);

                if (ZwReadVirtualMemory(hProcess, reinterpret_cast<void*>(regionStart), buffer.data(), bytesToRead, nullptr) != 0) {
                    break;
                }
                for (size_t begin = 0; begin <= bytesToRead - stringLength; begin++) {
                    if (buffer[begin] == stringtofind[0]) {
                        std::string stringbuffer(buffer.begin() + begin, buffer.begin() + begin + stringLength);

                        if (stringtofind == stringbuffer) {
                            addres.push_back(regionStart + begin);
                            //flag = false;
                            //return;
                        }
                    }
                }

                regionStart += bytesToRead;
            }
        }

        currentmemorypage = reinterpret_cast<char*>(currentmemorypage) + info.RegionSize;
    }

    currentmemorypage = nullptr;
}




std::vector<uintptr_t> scanner::returnaddreses()
{
    return addres;
}
