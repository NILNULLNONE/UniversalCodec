#pragma once

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
};
