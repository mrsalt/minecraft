#pragma once

#include <string>
#include <stdarg.h>
#include <stdio.h>

inline std::string format(const char * fmt, ...)
{
    char buffer[256];
    va_list va;
    va_start(va, fmt);
    vsprintf_s(buffer, sizeof(buffer), fmt, va);
    va_end(va);
    return std::string(buffer);
}
