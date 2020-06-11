#include <iostream>
#include <Windows.h>
#include "common.hpp"


int main()
{
    const std::string fileName{ "some_file" };
    HANDLE fileHandle = CreateFileA(fileName.c_str(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        std::stringstream message{ {"Could not open some_file."} };
        logERR(message);

        return -1;
    }

    const int bufferSize{ 255 };
    char buffer[bufferSize]{ };
    memset(buffer, 'A', bufferSize-1);

    DWORD nrBytesWrittenToFile{ };
    const BOOL errWriteFile = WriteFile(fileHandle, buffer, bufferSize, &nrBytesWrittenToFile, NULL);
    CloseHandle(fileHandle);
    if (errWriteFile == 0)
    {
        std::stringstream message{ {"Failed to write to some_file."} };
        logERR(message);

        return -1;
    }
    else
    {
        if (nrBytesWrittenToFile != bufferSize)
        {
            std::stringstream message{ };
            message << "Could only write " << nrBytesWrittenToFile << " out of " << bufferSize << " to some_file.";
            logERR(message);

            return -1;
        }
        else
        {
            std::cout << "Successfully wrote the information to some_file." << std::endl;
        }
    }
    
    fileHandle = CreateFileA(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        std::stringstream message{ {"Could not open some_file for reading."} };
        logERR(message);

        return -1;
    }

    const LONG distanceToMove{ 199 };
    const DWORD errSetFilePointer = SetFilePointer(fileHandle, distanceToMove, NULL, FILE_BEGIN);
    if (errSetFilePointer == distanceToMove)
    {
        const DWORD nrOfBytesToRead{ 55 };
        char outputFromReading[nrOfBytesToRead + 1]{ };
        DWORD nrOfBytesRead{ };
        
        const BOOL errReadFile{ ReadFile(fileHandle, outputFromReading, nrOfBytesToRead, &nrOfBytesRead, NULL) };
        if (errReadFile == 0)
        {
            std::stringstream message{ {"Failed to read from some_file."} };
            logERR(message);

            CloseHandle(fileHandle);
            return -1;
        }
        else
        {
            if (nrOfBytesToRead != nrOfBytesRead)
            {
                std::stringstream message{ };
                message << "Could only read " << nrOfBytesRead << " out of " << nrOfBytesToRead << " from some_file.";
                logERR(message);

                CloseHandle(fileHandle);
                return -1;
            }
            else
            {
                std::cout << "Successfully read the following information from some_file:" << std::endl;
                std::cout << outputFromReading << std::endl;
            }
        }
    }
    else
    {
        std::stringstream message{ {"Could not set the pointer before reading from some_file."} };
        logERR(message);

        CloseHandle(fileHandle);
        return -1;
    }

    CloseHandle(fileHandle);
    return 0;
}