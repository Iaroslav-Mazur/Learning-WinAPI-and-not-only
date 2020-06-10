#include <iostream>
#include <Windows.h>
#include <common.hpp>

int main()
{
    HANDLE mutex = CreateMutexA(NULL, true, "my mutex");
    if (mutex == NULL)
    {
        std::stringstream message{ {"Couldn't create a mutex: the API method has failed!"} };
        logERR(message);

        return -1;
    }
    else 
    {
        if (mutex && GetLastError() == ERROR_ALREADY_EXISTS)
        {
            std::stringstream message{ {"Another instance of this program is already running!"} };
            logERR(message);

            CloseHandle(mutex);
            return -1;
        }
        else
        {
            std::cout << "Working hard..." << std::endl;
            Sleep(10000);
            std::cout << "Stopped working hard..." << std::endl;
        }
    }

    CloseHandle(mutex);
    return 0;
}