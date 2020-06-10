#include <iostream>
#include <Windows.h>
#include "common.hpp"


int main()
{
    const std::string fileName{ "\\\\.\\PhysicalDrive0" };
    HANDLE fileHandle = CreateFileA(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        std::stringstream message{ {"Could not open C:"} };
        logERR(message); 
        
        return -1;
    }

    DISK_GEOMETRY pdg{ };
    DWORD bytesReturned{ 0 };
    BOOL errDeviceIoControl = DeviceIoControl(fileHandle,
                                                IOCTL_DISK_GET_DRIVE_GEOMETRY,
                                                NULL,
                                                0,
                                                &pdg,
                                                sizeof(pdg),
                                                &bytesReturned,
                                                NULL);
    CloseHandle(fileHandle);

    if (errDeviceIoControl == FALSE)
    {
        std::stringstream message{ {"The call to DeviceIoControl has failed!"} };
        logERR(message); 
        
        return -1;
    }
    else
    {
        std::cout << "Here's the geometry of PhysicalDrive0: " << std::endl;
        std::cout << "Cylinders: " << pdg.Cylinders.QuadPart << std::endl;
        std::cout << "MediaType: " << pdg.MediaType << std::endl;
        std::cout << "TracksPerCylinder: " << pdg.TracksPerCylinder << std::endl;
        std::cout << "SectorsPerTrack: " << pdg.SectorsPerTrack << std::endl;
        std::cout << "BytesPerSector: " << pdg.BytesPerSector << std::endl;
    }
    
    return 0;
}