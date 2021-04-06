#pragma once
#include "Array.h"

/*
auto GetBinaryStr = [](CSizeType InValue, CSizeType BitCnt)->char*
{
    static char Buffer[64];
    memset(Buffer, 0, sizeof(Buffer));
    // auto BitCnt = CMath::BitCount(InValue);
    for(int i = 0; i< BitCnt; ++i)
    {
        Buffer[BitCnt-i-1] = ( (InValue & (1<<i)) != 0? '1' : '0');
    }
    return Buffer;
};
*/

class CString
{
public:
    CString(const CCharType* InStr)
    {
        auto Len = strlen(InStr);
        if(Len > 0)
        {
            Chars.Resize(Len + 1);
            CMemory::Copy(Chars.GetData(), InStr, Len + 1);
        }
    }

    const CCharType* CStr() const
    {
        if(Chars.IsEmpty())return "";
        return &Chars[0];
    }

    void Append(const CCharType InChar)
    {
        Chars.Add(InChar);
    }

    void Concat(const CString& InStr)
    {
        Chars.Concat(InStr.Chars);
    }

    CString& operator=(const CString& InStr)
    {
        Chars = InStr.Chars;
        return *this;
    }

    CString operator+(const CCharType InChar) const 
    {
        CString Ret = *this;
        Ret.Append(InChar);
        return Ret;
    }

    CString operator+(const CString& InStr) const
    {
        CString Ret = *this;
        Ret.Concat(InStr);
        return Ret;
    }

    CString& operator+=(const CString& InStr)
    {
        Concat(InStr);
        return *this; 
    }

    CString& operator+=(const CCharType InChar)
    {
        Append(InChar);
        return *this;
    }
private:
    CArray<CCharType> Chars;
};

CString operator+(const CCharType InChar, const CString& InStr);
