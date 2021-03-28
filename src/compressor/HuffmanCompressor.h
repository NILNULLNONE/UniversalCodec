#pragma once
#include <cstdint>
#include "CompressorInterface.h"

class CHuffmanCompressor : public CCompressor
{
public:
    virtual void Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
    virtual void Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
};