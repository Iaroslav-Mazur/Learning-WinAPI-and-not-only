#include <iostream>
#include <Windows.h>
#include <common.hpp>

int main()
{
    SHELLEXECUTEINFO ShExecInfo{ 0 };
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.lpVerb = "open";
    ShExecInfo.lpFile = "C:\\Windows\\SysWow64\\cmd.exe";
    ShExecInfo.nShow = SW_SHOW;
    
    BOOL ret = ShellExecuteEx(&ShExecInfo);
    if (ret == false)
    {
        std::stringstream message{ {"The call to ShellExecuteEx() has failed!"} };
        logERR(message);

        CloseHandle(ShExecInfo.hProcess);
        return -1;
    }
    
    if (WaitForSingleObject(ShExecInfo.hProcess, INFINITE) != WAIT_OBJECT_0)
    {
        std::stringstream message{ {"The call to WaitForSingleObject() has failed!"} };
        logERR(message);

        CloseHandle(ShExecInfo.hProcess);
        return -1;
    }

    CloseHandle(ShExecInfo.hProcess);
    return 0;
}