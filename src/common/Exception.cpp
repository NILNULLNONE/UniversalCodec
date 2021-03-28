#include "Exception.h"
#include <cassert>
#include <cstdarg>
#include <cstdio>

namespace CException
{
    void Check(bool Exp)
    {
        assert(Exp);
    }

    void CheckFormatted(bool Exp, const char* Format, ...)
    {
        if (!Exp)
        {
            va_list Args;
            va_start(Args, Format);
            vprintf(Format, Args);
            va_end(Args);
            assert(0);
        }
    }
} // namespace Exception