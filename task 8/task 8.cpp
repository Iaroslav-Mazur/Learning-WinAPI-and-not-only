#include <iostream>
#include <Windows.h>
#include <common.hpp>

int main()
{
    LPVOID virtAllocResult{ VirtualAlloc(NULL, 512, MEM_COMMIT, PAGE_READWRITE) };
    if (virtAllocResult == NULL)
    {
        std::stringstream message{ {"Could not allocate the memory."} };
        logERR(message);

        return -1;
    }

    memset(virtAllocResult, 65, 511);
    std::cout << "Wrote to the allocated memory: " << (char *) virtAllocResult << std::endl;

    VirtualFree(virtAllocResult, NULL, MEM_RELEASE);

    return 0;
}