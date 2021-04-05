#include "common/TypeDef.h"
#include "common/Log.h"
#include "common/Memory.h"
#include "common/Exception.h"
#include "common/Array.h"
#include "common/File.h"
#include "codec/media/MediaCodecInterface.h"

#pragma pack(push)
#pragma pack(1)
struct CPNGHeader
{
    CByteType Mark89;                   // 89
    CByteType MarkPNG[3];               // 50 4E 47
    CByteType MarkDOSLineEnding[2];     // 0D 0A
    CByteType MarkStopDisplay;          // 1A
    CByteType MarkUnixLineEnding;       // 0A
};
#pragma pack(pop)

CSizeType ReadHeaderAndCheck(const CByteType *SrcData, CPNGHeader* OutHeader)
{
    auto Stride = CMemory::Read<CPNGHeader>(SrcData, *OutHeader);

    CByteType Target[8] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};
    CException::Check(CMemory::Compare(OutHeader, Target, sizeof(Target)) == 0);

    return Stride;
}

struct CPNGChunk
{
    // should swap to little endian (origin is big endian)
    CByteType ChunkLen[4]; 
    CByteType ChunkName[4];
    CByteType* ChunkData;
    CByteType CheckSum[4];
    CPNGChunk* Next;
    CSizeType GetDataLen() const {return *CMemory::ForceCast<CSizeType>(ChunkLen);}
    CSizeType GetSizeInByte() const { return sizeof(ChunkLen) + sizeof(ChunkName) + sizeof(CheckSum) + GetDataLen(); }
    bool IsCritical() const {return (ChunkName[0] & 0x20) == 0;}
    bool IsPublic() const {return (ChunkName[1] & 0x20) == 0;}
    bool IsValid() const {return (ChunkName[2] & 0x20) == 0;}
    // The chunk may be safely copied regardless of the extent of modifications to the file
    // Otherwise, it may only be copied if the modifications have not touched any critical chunks.
    bool IsSafeToCopy() const {return (ChunkName[3] & 0x20) != 0;}
};

CPNGChunk* ReadChunk(const CByteType *&SrcData)
{
    CPNGChunk* NewChunk = CMemory::Malloc<CPNGChunk>(1);
    NewChunk->Next = nullptr;
    SrcData += CMemory::Copy(NewChunk->ChunkLen, SrcData, sizeof(NewChunk->ChunkLen));
    CMemory::SwapEndian(NewChunk->ChunkLen, sizeof(NewChunk->ChunkLen));
    SrcData += CMemory::Copy(NewChunk->ChunkName, SrcData, sizeof(NewChunk->ChunkName));
    CSizeType ChunkLen = NewChunk->GetDataLen();
    NewChunk->ChunkData = CMemory::Malloc<CByteType>(ChunkLen);
    SrcData += CMemory::Copy(NewChunk->ChunkData, SrcData, ChunkLen);
    SrcData += CMemory::Copy(NewChunk->CheckSum, SrcData, sizeof(NewChunk->CheckSum));
    CLog::DebugLog("New Chunk: %c%c%c%c\n",
                   NewChunk->ChunkName[0],
                   NewChunk->ChunkName[1],
                   NewChunk->ChunkName[2],
                   NewChunk->ChunkName[3]
    );
    return NewChunk;
}

void ReadChunks(const CByteType *SrcData, CSizeType SrcLen,
        CPNGChunk *Chunks, CArray<CPNGChunk*> &CriticalChunks, CArray<CPNGChunk*>&AncillaryChunks)
{
    while(SrcLen > 0)
    {
        CPNGChunk* NewChunk = ReadChunk(SrcData);
        SrcLen -= NewChunk->GetSizeInByte();
        Chunks->Next = NewChunk;
        Chunks = NewChunk;
        if(NewChunk->IsCritical())CriticalChunks.Add(NewChunk);
        else AncillaryChunks.Add(NewChunk);
    }
}

#pragma pack(push)
#pragma pack(1)
struct CHDRChunk 
{
    enum EColorType
    {
        GreyScale = 0,          // L
        TrueColor = 2,          // RGB
        IndexedColor = 3,       // I
        GreyScaleAlpha = 4,     // L/A
        TrueColorAlpha = 6      // RGBA
    };
    CSizeType ImgWidth, ImgHeight;
    CByteType BitDepth;             // 1, 2, 4, 8, 16
    CByteType ColorType;
    CByteType CompressionMethod;
    CByteType FilterMethod;
    CByteType InterlaceMethod;
    CPNGChunk* Chunk;
    void Parse(CPNGChunk *InChunk)
    {
        Chunk = InChunk;
        CMemory::Copy(this, Chunk->ChunkData, sizeof(CHDRChunk) - sizeof(CPNGChunk *));
        CMemory::SwapEndian(ImgWidth);
        CMemory::SwapEndian(ImgHeight);
    }

    bool UsePalette() const 
    {
        return (EColorType)ColorType == EColorType::IndexedColor;
    }

    CByteType GetChannelNum() const 
    {
        auto Type = (EColorType) ColorType;
        switch (ColorType)
        {
        case GreyScale:
        case IndexedColor:  
            return 1;
        case GreyScaleAlpha:
            return 2;
        case TrueColor:
            return 3;
        case TrueColorAlpha:
            return 4;
        default:
            break;
        }
        CException::Check(0);
        return 0;
    }

    CByteType GetBitDepth() const 
    {
        return BitDepth;
    }

    bool IsRGBA() const {return (EColorType)ColorType == EColorType::TrueColorAlpha;}

    bool IsRGB() const {return (EColorType)ColorType == EColorType::TrueColor;}

    bool IsGrey() const {return (EColorType)ColorType == EColorType::GreyScale;}

    bool IsGreyAlpha() const { return (EColorType)ColorType == EColorType::GreyScaleAlpha; }

    CByteType GetPixelSizeInByte() const
    {
        return GetBitDepth() / 8 * GetChannelNum();
    }

    CSizeType GetImageWidth() const { return ImgWidth; }

    CSizeType GetImageHeight() const { return ImgHeight; }

    CSizeType GetScanlineSizeInByte() const {return GetImageWidth() * GetPixelSizeInByte();}
};

struct CPLTEChunk
{
    CPNGChunk *Data;
};

struct CIDATChunk
{
    CPNGChunk* Data;
};

struct CIENDChunk
{
    CPNGChunk *Data;
};
#pragma pack(pop)

struct CCriticalChunks
{
    CHDRChunk IHDRChunk;
    CPLTEChunk PLTEChunk;
    CArray<CIDATChunk> IDATChunk;
    CIENDChunk IENDChunk;
};

struct CAncillaryChunks
{

};

void ParseCriticalChunks(const CArray<CPNGChunk *> &InChunks, CCriticalChunks* OutChunks)
{
    for(int i = 0; i < InChunks.Count(); ++i)
    {
        CPNGChunk* Chunk = InChunks[i];
        if (CMemory::Equal(Chunk->ChunkName, "IHDR", sizeof(Chunk->ChunkName)))
        {
            CException::Check(i == 0);
            OutChunks->IHDRChunk.Parse(Chunk);
        }
        else if (CMemory::Equal(Chunk->ChunkName, "PLTE", sizeof(Chunk->ChunkName)))
        {
            OutChunks->PLTEChunk.Data = Chunk;
        }
        else if(CMemory::Equal(Chunk->ChunkName, "IDAT", sizeof(Chunk->ChunkName)))
        {
            OutChunks->IDATChunk.Add({Chunk});
        }
        else if(CMemory::Equal(Chunk->ChunkName, "IEND", sizeof(Chunk->ChunkName)))
        {
            OutChunks->IENDChunk.Data = Chunk;
        }
    }
}

struct CZlibBitStream
{
    CZlibBitStream(CArray<CByteType> &InData) : Data(InData) {}

    bool NextBit()
    {
        auto Ret = PeekBit();
        BitPos++;
        return Ret;
    }

    bool PeekBit(CSizeType Offset = 0)
    {
        auto Pos = BitPos + Offset;
        return (Data[Pos / 8] & (1 << (Pos % 8))) != 0;;
    }

    CSizeType NextBits(CSizeType BitCnt)
    {
        auto Ret = PeekBits(BitCnt);
        BitPos += BitCnt;
        return Ret;
    }

    CByteType NextByte()
    {
        return static_cast<CByteType>(NextBits(8));
    }

    CSizeType PeekBits(CSizeType BitCnt)
    {
        CException::Check(CMath::DivideAndRoundUp(BitCnt, 8u) <= sizeof(CSizeType));
        CSizeType Ret = 0;
        for (CSizeType i = 0; i < BitCnt; ++i)
        {
            Ret |= (((CSizeType)PeekBit(i)) << i);
        }
        return Ret;
    }

    void SkipRemainBits()
    {
        if( (BitPos & 0x07u) != 0)
        {
            BitPos = CMath::Align(BitPos, 8u);
        }
    }

    void SkipBits(CSizeType BitCnt)
    {
        BitPos += BitCnt;
    }

    void SkipBytes(CSizeType Len)
    {
        SkipBits(Len * 8u);
    }

    CByteType* Current()
    {
        return &Data[BitPos/8];
    }

private:
    CArray<CByteType> &Data;
    CSizeType BitPos = 0;
};

struct CZlibHuffmanTree
{
    virtual CSizeType DecodeLitLen(CZlibBitStream &Stream) = 0;
    virtual CSizeType DecodeDistance(CZlibBitStream &Stream)= 0;
};

struct CFixedHuffmanTree : public CZlibHuffmanTree
{
    virtual CSizeType DecodeLitLen(CZlibBitStream &BitStream) override
    {
        /*
            Lit Value    Bits        Codes
            ---------    ----        -----
            0 - 143       8          00110000 through 10111111
            144 - 255     9          110010000 through 111111111
            256 - 279     7          0000000 through 0010111
            280 - 287     8          11000000 through 11000111
        */
        CSizeType LitLen = BitStream.PeekBits(7);
        // decode literal/len
        {
            if(CMath::InRange(LitLen, 0x00u, 0x17u))   
            {
                BitStream.SkipBits(7);
                // 0000000~0010111 : 256 ~ 279
                LitLen = 256 + LitLen - 0x00u;
            }
            else 
            {
                LitLen = BitStream.PeekBits(8);
                if (CMath::InRange(LitLen, 0x30u, 0xBFu))
                {
                    BitStream.SkipBits(8);
                    // 00110000~10111111 : 0 ~ 143
                    LitLen = 0x00u + LitLen - 0x30u;
                }
                else if(CMath::InRange(LitLen, 0xC0u, 0xC7u))
                {
                    BitStream.SkipBits(8);
                    // 11000000~11000111 : 280 ~ 287
                    LitLen = 280 + LitLen - 0xC0u;
                }
                else
                {
                    LitLen = BitStream.PeekBits(9);
                    CException::Check(CMath::InRange(LitLen, 0x190u, 0x1FFu));
                    BitStream.SkipBits(9);
                    // 110010000~111111111 : 144 ~ 255
                    LitLen = 144 + LitLen - 0x190u;
                }
            }
        }
        return LitLen;
    }

    virtual CSizeType DecodeDistance(CZlibBitStream &Stream) override
    {
        return Stream.NextBits(5);
    }
};

struct CDynamicHuffmanTree
{
    struct CCodeNode
    {
        bool IsLeaf()
        {
            return !Left && !Right;
        }

        CCodeNode* Next(CBitType Bit)
        {
            CException::Check(!IsLeaf());
            if(Bit==0)
            {
                CException::Check(Left);
                return Left;
            }
            CException::Check(Right);
            return Right;
        }

        static void DebugLogImpl(CCodeNode* Node, CSizeType Depth)
        {
            if(!Node)return;
            for(int i = 0; i < Depth; ++i)CLog::DebugLog("\t");
            CLog::DebugLog("[%u]\n", Node->Value);
            for(int i = 0; i < Depth; ++i)CLog::DebugLog("\t");
            CLog::DebugLog("[L]\n");
            DebugLogImpl(Node->Left, Depth + 1);
            for(int i = 0; i < Depth; ++i)CLog::DebugLog("\t");
            CLog::DebugLog("[R]\n");
            DebugLogImpl(Node->Right, Depth + 1);
        }

        void DebugLog()
        {
            DebugLogImpl(this, 0);
        }

        CSizeType Value = 0;
        CCodeNode* Left = nullptr;
        CCodeNode* Right = nullptr;
    };
    CCodeNode* Root = nullptr;

    static void Insert(CCodeNode *Root, CSizeType Code, CSizeType Value, CByteType CodeLen)
    {
        if(CodeLen == 0)
        {
            Root->Value = Value;
            return;
        }
        CBitType Bit = ((Code & (1 << (CodeLen-1))) != 0);
        CCodeNode** Node = (Bit? &Root->Right : &Root->Left);
        if (!*Node)
        {
            (*Node) = new CCodeNode{};
        }
        Insert(*Node, Code, Value, CodeLen - 1);
    }

    static CDynamicHuffmanTree* Build(const CArray<CSizeType>& Values, const CArray<CByteType>& CodeLengths)
    {
        CException::Check(Values.Count() == CodeLengths.Count() && CodeLengths.Count() >= 1);

        CDynamicHuffmanTree* Tree = new CDynamicHuffmanTree;
        Tree->Root = new CCodeNode{};

        CByteType MaxBits = 0;
        for(int i = 0; i < CodeLengths.Count(); ++i)
            MaxBits = CMath::Max(MaxBits, CodeLengths[i]);

        CArray<CByteType> Counts(MaxBits+1, 0);
        for(int i = 0; i < CodeLengths.Count(); ++i)
            Counts[CodeLengths[i]]++;

        CArray<CSizeType> NextCode(MaxBits+1, 0);
        Counts[0] = 0;
        CSizeType Code = 0;
        for (int i = 1; i <= MaxBits; ++i) 
            NextCode[i] = Code = (Code + Counts[i - 1]) << 1;

        CSizeType MaxCode = 0;
        Values.ForEach([&](CSizeType Index, const auto& Value){MaxCode = CMath::Max(MaxCode, Value);});
        CArray<CSizeType> CodeLengthOf(MaxCode+1, 0);
        for(int i = 0; i < Values.Count(); ++i) 
            CodeLengthOf[Values[i]] = CodeLengths[i];

        CArray<CSizeType> Codes(MaxCode+1, 0);
        for (int ValueCode = 0; ValueCode <= MaxCode; ++ValueCode)
            if(CodeLengthOf[ValueCode] != 0)
                Codes[ValueCode] = NextCode[CodeLengthOf[ValueCode]]++;

        for(int i = 0; i < Values.Count(); ++i)
            if (CodeLengths[i] != 0)
                Insert(Tree->Root, Codes[Values[i]], Values[i], CodeLengths[i]);

        return Tree;
    }

    CSizeType Decode(CZlibBitStream& Stream)
    {
        CException::Check(Root);
        CCodeNode* Current = Root;
        while(!Current->IsLeaf())
        {
            CBitType Bit = (CBitType)Stream.NextBits(1);
            Current = Current->Next(Bit);
            CException::Check(Current);
            if(Current->IsLeaf())
            {
                return Current->Value;
            }
        }
        CException::Check(0);
        return 0;
    }
};

struct CDynamicHuffmanTrees : public CZlibHuffmanTree
{
    CDynamicHuffmanTree* LitLenTree = nullptr;
    CDynamicHuffmanTree* DistTree = nullptr;
    static CDynamicHuffmanTrees *Build(CZlibBitStream &Stream)
    {
        CDynamicHuffmanTrees *Trees = new CDynamicHuffmanTrees;

        // 5 Bits: HLIT, # of Literal/Length codes - 257 (257 - 286)
        CSizeType HLIT = Stream.NextBits(5);
        // 5 Bits: HDIST, # of Distance codes - 1 (1 - 32)
        CSizeType HDIST = Stream.NextBits(5);
        // 4 Bits: HCLEN, # of Code Length codes - 4 (4 - 19)
        CSizeType HCLEN = Stream.NextBits(4);

        /*
            (HCLEN + 4) x 3 bits: code lengths for the code length
            alphabet given just above, in the order: 16, 17, 18,
            0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
        */
        static CArray<CSizeType> CodeLengthsValues = 
        {
            16, 17, 18, 0, 8,
            7, 9, 6, 10, 5, 
            11, 4, 12, 3, 13,
            2, 14, 1, 15
        };
        CArray<CByteType> CodeLengthsForCodeLengths = {};
        for(int i = 0; i < HCLEN + 4; i++)
            CodeLengthsForCodeLengths.Add(Stream.NextBits(3));
        CDynamicHuffmanTree* CodeLengthsTree = CDynamicHuffmanTree::Build(CodeLengthsValues, CodeLengthsForCodeLengths);

        /*
            HLIT + 257 code lengths for the literal/length alphabet,
            encoded using the code length Huffman code
        */
        CArray<CSizeType> LitLenValues = {};
        CArray<CByteType> LitLenCodeLengths = {};
        auto AddCodeLens = [](CSizeType DecodedLen, CSizeType& ValueStart,
            CArray<CByteType>& Lens, CArray<CSizeType>& Values, CByteType& PreLen,
            CZlibBitStream& Stream, CSizeType CountLimit) -> bool
        {
            if(DecodedLen <= 15)
            {
                Lens.Add(DecodedLen);
                Values.Add(ValueStart++);
                PreLen = DecodedLen;
            }
            else if(DecodedLen <= 18)
            {
                CByteType RepeatValue = (DecodedLen == 16? PreLen : 0);
                // TODO: Should cahnge PreLen?
                PreLen = RepeatValue;
                CSizeType RepeatTimes = (DecodedLen == 16? (Stream.NextBits(2) + 3)
                                        : DecodedLen == 17? (Stream.NextBits(3) + 3)
                                        : (Stream.NextBits(7) + 11));
                for(int i = 0; i < RepeatTimes && Lens.Count() < CountLimit; ++i)
                {
                    Lens.Add(PreLen);
                    Values.Add(ValueStart++);
                }
            }
            else
            {
                CException::Check(0);
            }
            return Lens.Count() < CountLimit;
        };

        CByteType PreLen = 0;
        CSizeType ValueStart = 0;
        while(AddCodeLens(CodeLengthsTree->Decode(Stream), ValueStart, LitLenCodeLengths,
            LitLenValues, PreLen, Stream, HLIT+257));
        Trees->LitLenTree = CDynamicHuffmanTree::Build(LitLenValues, LitLenCodeLengths);

        /*
            HDIST + 1 code lengths for the distance alphabet,
            encoded using the code length Huffman code
        */
        CArray<CSizeType> DistValues = {};
        CArray<CByteType> DistCodeLength = {};
        PreLen = 0;
        ValueStart = 0;
        while(AddCodeLens(CodeLengthsTree->Decode(Stream), ValueStart, DistCodeLength,
            DistValues, PreLen, Stream, HDIST + 1));
        Trees->DistTree = CDynamicHuffmanTree::Build(DistValues, DistCodeLength);

        delete CodeLengthsTree;

        return Trees;
    }

    virtual CSizeType DecodeLitLen(CZlibBitStream &Stream) override
    {
        CException::Check(LitLenTree != nullptr);
        return LitLenTree->Decode(Stream);
    }

    virtual CSizeType DecodeDistance(CZlibBitStream &Stream) override
    {
        CException::Check(DistTree != nullptr);
        return DistTree->Decode(Stream);
    }
};

void DecodeIDATChunks(const CArray<CIDATChunk>& IDATChunks, CArray<CByteType>& OutData)
{
    CArray<CByteType> DATChunk = {};
    for(int i = 0; i < IDATChunks.Count(); ++i)
    {
        DATChunk.Concat(IDATChunks[i].Data->ChunkData, IDATChunks[i].Data->GetDataLen());
    }

    CZlibBitStream BitStream(DATChunk);

    // Zlib header
    {
        CByteType CMF = BitStream.NextByte();
        CByteType FLG = BitStream.NextByte();
        // TODO: check FDICT?
        CException::Check( (((CSizeType)CMF)*256 + (CSizeType)FLG) % 31 == 0 );
    }

    bool IsLastBlock = false;
    CSizeType ChunkCnt = 0;
    while(!IsLastBlock)
    {
        // block header
        IsLastBlock = BitStream.NextBit();
        int CompressedType = BitStream.NextBits(2);
        CLog::DebugLog("Chunk #%u, IsLast: %d, Compress: %d\n", 
            ChunkCnt++, (int)IsLastBlock, CompressedType);
        // no compress
        if(CompressedType == 0)
        {
            BitStream.SkipRemainBits();
            CSizeType Len = BitStream.NextBits(32);
            CSizeType NLen = BitStream.NextBits(32);
            CException::CheckFormatted(Len & NLen == 0, "%x & %x != 0\n", Len, NLen);
            OutData.Concat(BitStream.Current(), Len);
            BitStream.SkipBytes(Len);
            continue;
        }
        // error
        else if (CompressedType == 3)
        {
            CException::CheckFormatted(0, "Error compress type!");
            return;
        }

        CZlibHuffmanTree* HuffmanTree = nullptr;
        if(CompressedType == 2)
        {
            HuffmanTree = CDynamicHuffmanTrees::Build(BitStream);
        }
        else if(CompressedType == 1)
        {
            HuffmanTree = new CFixedHuffmanTree;
        }

        CException::Check(HuffmanTree != nullptr);

        bool IsBlockEnd = false;
        while(!IsBlockEnd)
        {
            CSizeType LitLen = HuffmanTree->DecodeLitLen(BitStream);
            CException::Check(CMath::InRange(LitLen, 0u, 285u));
            if(LitLen < 256)
            {
                OutData.Add(LitLen);
            }
            else if(LitLen == 256)
            {
                IsBlockEnd = true;
            }
            else
            {
                /*
                        Extra               Extra               Extra
                    Code Bits Length(s) Code Bits Lengths   Code Bits Length(s)
                    ---- ---- ------     ---- ---- -------   ---- ---- -------
                    257   0     3       267   1   15,16     277   4   67-82
                    258   0     4       268   1   17,18     278   4   83-98
                    259   0     5       269   2   19-22     279   4   99-114
                    260   0     6       270   2   23-26     280   4  115-130
                    261   0     7       271   2   27-30     281   5  131-162
                    262   0     8       272   2   31-34     282   5  163-194
                    263   0     9       273   3   35-42     283   5  195-226
                    264   0    10       274   3   43-50     284   5  227-257
                    265   1  11,12      275   3   51-58     285   0    258
                    266   1  13,14      276   3   59-66
                */
                static CSizeType LengthBaseTable[29][2] =
                {
                    {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
                    {0, 8}, {0, 9}, {0, 10}, {1, 11}, {1, 13},

                    {1, 15}, {1, 17}, {2, 19}, {2, 23}, {2, 27},
                    {2, 31}, {3, 35}, {3, 43}, {3, 51}, {3, 59},

                    {4, 67}, {4, 83}, {4, 99}, {4, 115}, {5, 131},
                    {5, 163}, {5, 195}, {5, 227}, {0, 258}
                };

                auto &LengthBase = LengthBaseTable[LitLen - 257];
                CSizeType ExtraBits = LengthBase[0];
                CSizeType Length = LengthBase[1] + BitStream.NextBits(ExtraBits);

                /*
                      Extra           Extra               Extra
                 Code Bits Dist  Code Bits   Dist     Code Bits Distance
                 ---- ---- ----  ---- ----  ------    ---- ---- --------
                   0   0    1     10   4     33-48    20    9   1025-1536
                   1   0    2     11   4     49-64    21    9   1537-2048
                   2   0    3     12   5     65-96    22   10   2049-3072
                   3   0    4     13   5     97-128   23   10   3073-4096
                   4   1   5,6    14   6    129-192   24   11   4097-6144
                   5   1   7,8    15   6    193-256   25   11   6145-8192
                   6   2   9-12   16   7    257-384   26   12  8193-12288
                   7   2  13-16   17   7    385-512   27   12 12289-16384
                   8   3  17-24   18   8    513-768   28   13 16385-24576
                   9   3  25-32   19   8   769-1024   29   13 24577-32768
                */
                static CSizeType DistBaseTable[30][2] =
                {
                    {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 5},
                    {1, 7}, {2, 9}, {2, 13}, {3, 17}, {3, 25},

                    {4, 33}, {4, 49}, {5, 65}, {5, 97}, {6, 129},
                    {6, 193}, {7, 257}, {7, 385}, {8, 513}, {8, 769},

                    {9, 1025}, {9, 1537}, {10, 2049}, {10, 3073}, {11, 4097},
                    {11, 6145}, {12, 8193}, {12, 12289}, {13, 16385}, {13, 24577}
                };

                CSizeType Dist = HuffmanTree->DecodeDistance(BitStream);
                CException::Check(CMath::InRange(Dist, 0u, 29u));
                auto &DistBase = DistBaseTable[Dist];
                ExtraBits = DistBase[0];
                CSizeType Distance = DistBase[1] + BitStream.NextBits(ExtraBits);
                for(int i = 0; i < Length; ++i)
                {
                    CByteType Byte = *(OutData.End() - Distance);
                    OutData.Add(Byte);
                }
            }
        }
    }
}


struct CPNGScanlines
{
    CArray<CByteType*> Lines = {};
    CSizeType ScanlineLenInByte;
};

enum class EFilterType
{
    None = 0,
    Sub = 1,
    Up = 2, 
    Average = 3,
    Paeth = 4
};

void Unfilter(CArray<CByteType>& UnzipedData, CPNGScanlines &OutScanlines)
{
    CByteType* DataPtr = UnzipedData.GetData();
    CSizeType Stride = 0;
    do
    {
        CByteType FilterType;
        Stride += CMemory::Read<CByteType>(DataPtr + Stride, FilterType);
        EFilterType Type = (EFilterType)FilterType;
        // TODO: Filter
        CException::Check(Type == EFilterType::None);
        OutScanlines.Lines.Add(DataPtr + Stride);
        Stride += OutScanlines.ScanlineLenInByte;
    } while (Stride < UnzipedData.Count());
}

void DebugCriticalChunks(const CCriticalChunks& InChunks)
{
    {
        auto &IHDRChunk = InChunks.IHDRChunk;
        FILE* IHDRFile = fopen("IHDR.txt", "w");
        fprintf(IHDRFile, "Width: %u\nHeight: %u\n", IHDRChunk.ImgWidth, IHDRChunk.ImgHeight);
        fprintf(IHDRFile, "BitDepth: %u\nColorType: %u\n", (CSizeType)IHDRChunk.BitDepth, (CSizeType)IHDRChunk.ColorType);
        fprintf(IHDRFile, "Compress: %u\nFilter: %u\n", (CSizeType)IHDRChunk.CompressionMethod, (CSizeType)IHDRChunk.FilterMethod);
        fprintf(IHDRFile, "Interlace: %u\n", (CSizeType)IHDRChunk.InterlaceMethod);
        fclose(IHDRFile);
    }

    {
        CException::Check(!InChunks.IDATChunk.IsEmpty());
        FILE* ConcatedIDATFile = fopen("IDAT.txt", "wb");
        for(int i = 0; i < InChunks.IDATChunk.Count(); ++i)
        {
            auto& IDATChunk = InChunks.IDATChunk[i];
            char Filename[1024];
            sprintf(Filename, "IDAT_%d.txt", i);
            CFile::SaveDataToFile(Filename, IDATChunk.Data->ChunkData, IDATChunk.Data->GetDataLen());
            fwrite(IDATChunk.Data->ChunkData, sizeof(CByteType), IDATChunk.Data->GetDataLen(), ConcatedIDATFile);
        }
        fclose(ConcatedIDATFile);
    }

    {
        CArray<CByteType> IDATRawData = {};
        DecodeIDATChunks(InChunks.IDATChunk, IDATRawData);
        CFile::SaveDataToFile("IDAT_raw.dat", IDATRawData.GetData(), IDATRawData.SizeInBytes());

        if (InChunks.IHDRChunk.IsRGBA() || InChunks.IHDRChunk.IsRGB())
        {
            CPNGScanlines Scanlines;
            auto& HDRChk = InChunks.IHDRChunk;
            Scanlines.ScanlineLenInByte = HDRChk.GetScanlineSizeInByte();
            Unfilter(IDATRawData, Scanlines);
            FILE* PPMFile = fopen("Raw.ppm", "w");
            fprintf(PPMFile, "P3\n");
            fprintf(PPMFile, "%u %u\n", HDRChk.GetImageWidth(), HDRChk.GetImageHeight());
            fprintf(PPMFile, "%u\n", 255);
            for(int i = 0; i < Scanlines.Lines.Count(); ++i)
            {
                for(int j = 0; j < HDRChk.GetImageWidth(); ++j)
                {
                    for(int k = 0; k < 3; ++k)
                    {
                        fprintf(PPMFile, "%u ", (CSizeType)Scanlines.Lines[i][j*HDRChk.GetChannelNum()+k]);
                    }
                }
                fprintf(PPMFile, "\n");
            }
            fclose(PPMFile);
        }
    }
}

void Decode(const CByteType* SrcData, const CSizeType SrcLen,
        CByteType*& DstData, CSizeType& DstLen
    )
{
    CSizeType Stride = 0, RemainLen = SrcLen;
    const CByteType* DatPos = SrcData;

    CPNGHeader Header;
    Stride = ReadHeaderAndCheck(DatPos, &Header);
    DatPos += Stride;
    RemainLen -= Stride;

    CPNGChunk *ChunkHead = CMemory::Malloc<CPNGChunk>(1);
    CArray<CPNGChunk*> CriticalChunks = {}, AncillaryChunks = {};
    ChunkHead->Next = nullptr;
    ReadChunks(DatPos, RemainLen, ChunkHead, CriticalChunks, AncillaryChunks);

    CCriticalChunks ParsedCriticalChunks;
    ParseCriticalChunks(CriticalChunks, &ParsedCriticalChunks);

    {
        DebugCriticalChunks(ParsedCriticalChunks);
    }
}

void Encode(const CByteType *SrcData, const CSizeType SrcLen,
            CByteType *&DstData, CSizeType &DstLen
    )
{
}

void CPNGCodec::DecodeFile(const char* Filename)
{
    CByteType* Data = nullptr;
    CSizeType DataLen;
    CFile::LoadDataFromFile(Filename, Data, DataLen);
    CByteType* DecodedData = nullptr;
    CSizeType DecodedLen = 0;
    Decode(Data, DataLen, DecodedData, DecodedLen);
}