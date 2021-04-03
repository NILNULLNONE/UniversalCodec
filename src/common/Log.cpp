#include "Log.h"

namespace CLog
{
    bool GEnable = true;
    void DebugLog(const char *Format, ...)
    {
#ifndef NDEBUG
#ifndef NLOG 
        if(!GEnable)return;
        va_list Args;
        va_start(Args, Format);
        vprintf(Format, Args);
        va_end(Args);
#endif
#endif
    }

    void Enable()
    {
        GEnable = true;
    }

    void Disable()
    {
        GEnable = false;
    }

    void Toggle()
    {
        GEnable = !GEnable;
    }
}