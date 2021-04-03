#pragma once
#include "common/TypeDef.h"

class CCompressor
{
public:
    virtual void Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) = 0;
    virtual void Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) = 0;
};

// #define DefineCompressor(CompressorName) \
// class CompressorName : public CCompressor \
// { \
// public: \
//     virtual void Compress(CByteType * &DstData, CSizeType & DstLen, const CByteType *SrcData, const CSizeType SrcLen) override; \
//     virtual void Decompress(CByteType * &DstData, CSizeType & DstLen, const CByteType *SrcData, const CSizeType SrcLen) override; \
// };

// DefineCompressor(CHuffmanCompressor);
// DefineCompressor(CLZ77Compressor);
// #undef DefineCompressor

class CHuffmanCompressor : public CCompressor
{
public:
    virtual void Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
    virtual void Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
};
class CLZ77Compressor : public CCompressor
{
public:
    virtual void Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
    virtual void Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
};

class CLZ78Compressor : public CCompressor
{
public:
    virtual void Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
    virtual void Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
};

class CLZ4Compressor : public CCompressor
{
public:
    virtual void Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
    virtual void Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
};

class CLZOCompressor : public CCompressor
{
public:
    virtual void Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
    virtual void Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
};

class CLZWCompressor : public CCompressor
{
public:
    virtual void Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
    virtual void Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
};

class CLZSSCompressor : public CCompressor
{
public:
    virtual void Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
    virtual void Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
};

class CDeflateCompressor : public CCompressor
{
public:
    virtual void Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
    virtual void Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
};

class CRLECompressor : public CCompressor
{
public:
    virtual void Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
    virtual void Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
};

class CBISECompressor : public CCompressor
{
public:
    virtual void Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
    virtual void Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen) override;
};
