#pragma once
#include "common/TypeDef.h"

class CCompressor
{
public:
    virtual void Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) = 0;
    virtual void Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) = 0;
};