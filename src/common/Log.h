#pragma once
#include <cstdio>
#include <cstdarg>

namespace CLog
{
    void DebugLog(const char* Format, ...)
    {
    #ifndef NDEBUG
        va_list Args;
        va_start(Args, Format);
        vprintf(Format, Args);
        va_end(Args);
    #endif
    }
}