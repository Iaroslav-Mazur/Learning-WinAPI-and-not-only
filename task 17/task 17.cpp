#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <common.hpp>

int main()
{
    HANDLE processesSnapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL) };
    if (processesSnapshot == INVALID_HANDLE_VALUE)
    {
        std::stringstream message{ {"Couldn't obtain a handle to the snapshot of the running processes!"} };
        logERR(message);

        return -1;
    }

    PROCESSENTRY32 entry{ };
    entry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(processesSnapshot, &entry) == false)
    {
        CloseHandle(processesSnapshot);
        return -1;
    }

    const std::string processToBeKilled{ "Calculator.exe" };
    bool processFound{ false };

    do
    {
        if (strcmp(entry.szExeFile, processToBeKilled.c_str()) == 0)
        {
            processFound = true;
            HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, false, entry.th32ProcessID);
            if (processHandle == INVALID_HANDLE_VALUE)
            {
                std::stringstream message{ };
                message << "Couldn't obtain a handle to the " << processToBeKilled.c_str() << " process!\n";
                logERR(message);

                CloseHandle(processesSnapshot);
                return -1;
            }

            if (TerminateProcess(processHandle, 0) == 0)
            {
                std::stringstream message{ };
                message << "Couldn't terminate the " << processToBeKilled.c_str() << " process!\n";
                logERR(message);

                CloseHandle(processHandle);
                CloseHandle(processesSnapshot);
                return -1;
            }

            std::cout << processToBeKilled.c_str() << "has been successfully killed!\n";

            CloseHandle(processHandle);
        }
    } while (Process32Next(processesSnapshot, &entry));

    if (!processFound)
    {
        std::cout << processToBeKilled.c_str() << " was not found in the list of the running processes!\n";
    }

    CloseHandle(processesSnapshot);
    return 0;
}