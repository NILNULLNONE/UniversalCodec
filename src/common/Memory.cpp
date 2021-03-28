#include "Memory.h"

namespace CMemory
{
    CSizeType Copy(void *Dst, const void *Src, CSizeType Size)
    {
        memcpy(Dst, Src, static_cast<size_t>(Size));
        return Size;
    }

    void Zero(void *Dst, CSizeType InLen)
    {
        memset(Dst, 0, InLen);
    }
}