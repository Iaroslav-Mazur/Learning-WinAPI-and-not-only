#include <iostream>
#include <iomanip>
#include <atlbase.h>
#include <Windows.h>

bool fileExists(LPCSTR relativePath)
{
    const DWORD dwAttrib{ GetFileAttributesA(relativePath) };
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

DWORD CALLBACK progressCallback(LARGE_INTEGER TotalFileSize,
                                LARGE_INTEGER TotalBytesTransferred,
                                LARGE_INTEGER StreamSize,
                                LARGE_INTEGER StreamBytesTransferred,
                                DWORD dwStreamNumber,
                                DWORD dwCallbackReason,
                                HANDLE hSourceFile,
                                HANDLE hDestinationFile,
                                LPVOID lpData)
{
    const double percentageDone{ ((double)TotalBytesTransferred.QuadPart / TotalFileSize.QuadPart) * 100 };
    std::cout << std::setprecision(4) << percentageDone << "% copied." << std::endl;
    
    return PROGRESS_CONTINUE;
}

void copyUsingCopyFileEx(LPCSTR existingFileName, LPCSTR newFileName)
{
    if (fileExists(existingFileName))
    {
        if (!fileExists(newFileName))
        {
            std::cout << "Started copying the file." << std::endl;
            CopyFileExA(existingFileName, newFileName, &progressCallback, NULL, FALSE, 0);
            std::cout << "The file has been successfully copied!" << std::endl;
        }
        else
        {
            std::cerr << "Error: The copy of the specified source file is already present!" << std::endl;
        }
    }
    else
    {
        std::cerr << "Error: The specified source file does not exist!" << std::endl;
    }
}

int main()
{
    copyUsingCopyFileEx("earth_original.png", "earth_copy.png");
    return 0;
}