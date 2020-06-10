#include <iostream>
#include <Windows.h>
#include <common.hpp>

int main()
{
    WIN32_FIND_DATA data{ };
    HANDLE rootHandle = FindFirstFile("C:\\*", &data);
    
    if (rootHandle == INVALID_HANDLE_VALUE) {
        std::stringstream message{ {"Couldn't obtain a handle to the root directory!"} };
        logERR(message);

        return -1;
    }
    else
    {
        std::cout << data.cFileName << std::endl;
    }

    while (FindNextFile(rootHandle, &data))
    {
        std::cout << data.cFileName << std::endl;
    }

    FindClose(rootHandle);
}