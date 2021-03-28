#pragma once

namespace CException
{
    void Check(bool Exp);

    void CheckFormatted(bool Exp, const char* Format, ...);
} // namespace Exception
