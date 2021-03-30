#include "Log.h"

namespace CLog
{
    bool GEnable = true;
    void DebugLog(const char *Format, ...)
    {
#ifndef NDEBUG
        if(!GEnable)return;
        va_list Args;
        va_start(Args, Format);
        vprintf(Format, Args);
        va_end(Args);
#endif
    }

    void TurnOn()
    {
        GEnable = true;
    }

    void TurnOff()
    {
        GEnable = false;
    }

    void Toggle()
    {
        GEnable = !GEnable;
    }
}