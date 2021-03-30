#pragma once
#include <cstdio>
#include <cstdarg>

namespace CLog
{
    extern bool GEnable;
    void DebugLog(const char* Format, ...);
    void TurnOn();
    void TurnOff();
    void Toggle();
}