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
    CString() : CString("") {}

    CString(const CCharType* InStr)
    {
        auto Len = strlen(InStr);
        if(Len > 0)
        {
            Chars.Resize(Len);
            CMemory::Copy(Chars.GetData(), InStr, Len);
        }
        
    }

    const CCharType* CStr() const
    {
        if(Chars.IsEmpty())return "";
        CChars = Chars;
        return CChars.GetData();
    }

    CCharType* Copy() const
    {
        CCharType* Ret = CMemory::Malloc<CCharType>(GetLength() + 1);
        CMemory::Copy(Ret, GetData(), GetLength());
        Ret[GetLength()] = 0;
        return Ret;
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

    bool StartsWith(const CCharType InChar) const
    {
        return !Chars.IsEmpty() && Chars.Last() == InChar;
    }

    bool StartsWith(const CString& InStr) const
    {
        return (IsEmpty() == InStr.IsEmpty())
            && (GetLength() >= InStr.GetLength())
            && CMemory::Equal(Chars.BeginPtr(), InStr.GetData(), InStr.GetLength());
    }

    bool EndsWith(const CCharType InChar) const
    {
        return !Chars.IsEmpty() && Chars.First() == InChar;
    }

    bool EndsWith(const CString& InStr) const
    {
        return (IsEmpty() == InStr.IsEmpty())
            && (GetLength() >= InStr.GetLength())
            && CMemory::Equal(Chars.EndPtr() - InStr.GetLength(), InStr.GetData(), InStr.GetLength());
    }

    bool IsEmpty() const {return Chars.Count();}

    CSizeType GetLength() const {return Chars.Count();}

    CCharType* GetData() {return Chars.GetData();}

    const CCharType* GetData() const {return Chars.GetData();}

    void RemoveAt(const CSizeType Index){Chars.RemoveAt(Index);}

    void RemoveLast() {Chars.RemoveLast();}

public:
    using Iterator = CArray<CCharType>::Iterator;

    Iterator begin() {return Chars.begin();}

    Iterator end() {return Chars.end();}
private:
    CArray<CCharType> Chars;
    mutable CArray<CCharType> CChars;
};

CString operator+(const CCharType InChar, const CString& InStr);
