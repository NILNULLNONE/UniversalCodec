#pragma once
#include <cstdio>
#include <cstdarg>

namespace CLog
{
    extern bool GEnable;
    void DebugLog(const char* Format, ...);
    char* BinaryString();
    void TurnOn();
    void TurnOff();
    void Toggle();
}