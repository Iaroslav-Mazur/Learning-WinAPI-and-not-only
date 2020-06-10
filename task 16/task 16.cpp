#pragma warning(disable : 4996)
#include <iostream>
#include <Windows.h>

int main()
{
    OSVERSIONINFO osvi{ };
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);

    BOOL isWindows7 = ((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion == 1));
    if (isWindows7)
    {
        std::cout << "It's Win7 we're running on here!" << std::endl;
    }
    else
    {
        std::cout << "The version of the OS we're running is anything BUT Win7!" << std::endl;
    }

    return 0;
}