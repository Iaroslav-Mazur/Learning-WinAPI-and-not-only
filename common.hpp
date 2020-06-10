#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <sstream>

void logERR(const std::stringstream& message)
{
    std::cerr << message.str() << std::endl;
    char err[256]{ };
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err, 255, NULL);
    std::cerr << "Error code: " << err << std::endl;
}

#endif COMMON_H