#include "CompressorInterface.h"
#include "common/Array.h"

void CRLECompressor::Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen)
{
    CArray<CByteType> Result = {};
    CSizeType Iter = 0;
    const CSizeType MaxCount = 255;
    while(Iter < SrcLen)
    {
        CByteType Count = 1;
        CByteType StartByte = SrcData[Iter];
        while (Count < MaxCount && Iter + 1 < SrcLen && SrcData[Iter] == SrcData[Iter + 1])
        {
            ++Iter;
            ++Count;
        }
        ++Iter;
        Result.AddElements({Count, StartByte});
    }
    if(Result.IsEmpty())return;
    Result.DiscardTo(DstData, DstLen);
}

void CRLECompressor::Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen)
{
    CException::CheckFormatted(SrcLen % 2 == 0, "Data size is wrong.");
    CArray<CByteType>Result = {};
    CSizeType Iter = 0;
    while(Iter + 1 < SrcLen)
    {
        CByteType Count = SrcData[Iter];
        CByteType Byte = SrcData[Iter+1];
        while(Count--)Result.Add(Byte);
        Iter+=2;
    }
    if(Result.IsEmpty())return;
    Result.DiscardTo(DstData, DstLen);
}