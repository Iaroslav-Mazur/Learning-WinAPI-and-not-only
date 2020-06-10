#include <iostream>
#include <Windows.h>
#include "common.hpp"


struct BPBTable {
    WORD BytesPerSector{ };
    BYTE SectorsPerCluster{ };
    WORD ReservedSectors{ };
    BYTE filler1[3]{ };
    WORD NotUsedByNTFS1{ };
    BYTE MediaDescriptor{ };
    BYTE filler2{ };
    WORD SectorsPerTrack{ };
    WORD NumberOfHeads{ };
    DWORD HiddenSector{ };
    DWORD NotUsedByNTFS2{ };
};

int main()
{
    std::string fileName{ "\\\\.\\C:" };
    HANDLE fileHandle = CreateFileA(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        std::stringstream message{ {"Could not open C:"} };
        logERR(message);
        
        return -1;
    }

    const DWORD nrOfBytesToRead{ 512 };
    char outputFromReading[nrOfBytesToRead + 1]{ };
    DWORD nrOfBytesRead{ };

    BOOL errReadFile{ ReadFile(fileHandle, outputFromReading, nrOfBytesToRead, &nrOfBytesRead, NULL) };
    if (errReadFile == 0)
    {
        std::stringstream message{ {"Failed to read from C:"} };
        logERR(message);
        
        CloseHandle(fileHandle);
        return -1;
    }
    else
    {
        if (nrOfBytesToRead != nrOfBytesRead)
        {
            std::stringstream message{ };
            message << "Could only read " << nrOfBytesRead << " out of " << nrOfBytesToRead << " from C:.";
            logERR(message);

            CloseHandle(fileHandle);
            return -1;
        }
        else
        {
            std::cout << "Successfully read the following information from C: :" << std::endl;
            std::cout << outputFromReading << std::endl;
        }
    }
    
    CloseHandle(fileHandle);

    fileName = "my.dump";
    HANDLE dumpFileHandle = CreateFileA(fileName.c_str(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (dumpFileHandle == INVALID_HANDLE_VALUE)
    {
        std::stringstream message{ {"Could not open my.dump."} };
        logERR(message);
        
        return -1;
    }

    DWORD nrBytesWrittenToFile{ };
    BOOL errWriteFile = WriteFile(dumpFileHandle, outputFromReading, nrOfBytesToRead, &nrBytesWrittenToFile, NULL);
    if (errWriteFile == 0)
    {
        std::stringstream message{ {"Failed to write to my.dump."} };
        logERR(message);
        
        CloseHandle(dumpFileHandle);
        return -1;
    }
    else
    {
        if (nrBytesWrittenToFile != nrOfBytesToRead)
        {
            std::stringstream message{ };
            message << "Could only write " << nrBytesWrittenToFile << " out of " << nrOfBytesToRead << " to my.dump.";
            logERR(message);

            CloseHandle(dumpFileHandle);
            return -1;
        }
        else
        {
            std::cout << "Successfully wrote the information to my.dump." << std::endl;
        }
    }

    CloseHandle(dumpFileHandle);
    BPBTable table{ };
    memcpy(&table, outputFromReading + 3 + sizeof(LONGLONG), 25);
    std::cout << "Saved the BPB information to a corresponding structure." << std::endl;

    fileName = "\\\\.\\C:";
    fileHandle = CreateFileA(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        std::stringstream message{ {"Could not open C:"} };
        logERR(message);

        return -1;
    }

    const LONG distanceToMove{ 1 };
    const DWORD errSetFilePointer = SetFilePointer(fileHandle, distanceToMove, NULL, FILE_BEGIN);
    if (errSetFilePointer != distanceToMove)
    {
        if (GetLastError() == ERROR_INVALID_PARAMETER)
        {
            std::stringstream message{ {"Could not set the pointer for C: to an offset that is not sector-aligned!"} };
            logERR(message);
        }
        else
        {
            std::stringstream message{ {"Could not set the pointer before reading from C:."} };
            logERR(message);
        }
        
        CloseHandle(fileHandle);
        return -1;
    }

    memset(outputFromReading, 0, nrOfBytesToRead + 1);
    errReadFile = ReadFile(fileHandle, outputFromReading, nrOfBytesToRead, &nrOfBytesRead, NULL);
    if (errReadFile == 0)
    {
        std::stringstream message{ {"Failed to read from C:."} };
        logERR(message); 
        
        CloseHandle(fileHandle);
        return -1;
    }
    else
    {
        if (nrOfBytesToRead != nrOfBytesRead)
        {
            std::stringstream message{ };
            message << "Could only read " << nrOfBytesRead << " out of " << nrOfBytesToRead << " from C:.";
            logERR(message);

            CloseHandle(fileHandle);
            return -1;
        }
        else
        {
            std::cout << "Successfully read the following information from C: :" << std::endl;
            std::cout << outputFromReading << std::endl;
        }
    }

    CloseHandle(fileHandle);
    return 0;
}