#include "String.h"

CString operator+(const CCharType InChar, const CString& InStr)
{
    return InStr + InChar;
}
