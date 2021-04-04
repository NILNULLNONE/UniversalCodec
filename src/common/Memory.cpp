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

    int Compare(const void *Buf1, const void *Buf2, CSizeType Size)
    {
        return memcmp(Buf1, Buf2, (size_t)Size);
    }

    bool Equal(const void *Buf1, const void *Buf2, CSizeType Size)
    {
        return Compare(Buf1, Buf2, Size) == 0;
    }

    void SwapEndian(void *Data, CSizeType DataLen)
    {
        CSizeType Left = 0, Right = DataLen - 1;
        CByteType* Bytes = static_cast<CByteType*>(Data);
        while(Left < Right)
        {
            Swap(Bytes[Left++], Bytes[Right--]);
        }
    }

}