#include <climits>
#include "HuffmanCompressor.h"
#include "common/Heap.h"
#include "common/Log.h"

struct CHuffmanNode
{
    CHuffmanNode* Left = nullptr, *Right = nullptr;
    int Value = 0;
    int Weight = 0;
    CBitType EncodedBit = 0;
    bool IsLeaf() const {return !Left && !Right;}
    CHuffmanNode* GetChildByBit(CBitType InBit)
    {
        if(Left && Left->EncodedBit == InBit)return Left;
        if(Right && Right->EncodedBit == InBit)return Right;
        return nullptr;
    }

    void Delete()
    {
        if(Left)Left->Delete();
        if(Right)Right->Delete();
        delete this;
    }
};

struct CHuffmanNodeComparator
{
    bool operator()(const CHuffmanNode *Left, const CHuffmanNode *Right) const
    {
        return Left->Weight > Right->Weight;
    }
};

struct CBitSequenceTable
{
    using EntryCountType = uint16_t;
    CBitSequence TableData[UCHAR_MAX+1];
    CSizeType Serialize(int8_t* Dst)
    {
        CSizeType Step = 0;
        Step += CMemory::Write<EntryCountType>(Dst + Step, static_cast<EntryCountType>(GetEntryCount()));
        ForEachSequence([&Step, Dst](CSizeType InIndex, const CBitSequence &InSequence) {
            if(InSequence.IsEmpty())
                return;
            Step += CMemory::Write<uint8_t>(Dst + Step, static_cast<uint8_t>(InIndex));
            Step += InSequence.Serialize(Dst + Step);
        });
        return Step;
    }

    CSizeType Deserialize(const int8_t* Dst)
    {
        CSizeType Step = 0;
        EntryCountType EntryCount = 0;
        Step += CMemory::Read<EntryCountType>(Dst + Step, EntryCount);
        for(int i = 0; i < EntryCount; ++i)
        {
            uint8_t Index = 0;
            Step += CMemory::Read<uint8_t>(Dst + Step, Index);
            Step += TableData[Index].Deserialize(Dst + Step);
        }
        return Step;
    }

    CSizeType GetSerializeSizeInByte()
    {
        CSizeType Size = sizeof(EntryCountType);
        ForEachSequence([this, &Size](CSizeType Index, const CBitSequence& Sequence)
        {
            Size += sizeof(uint8_t) + Sequence.GetSerializeSizeInByte();
        });
        return Size;
    }

    void Add(uint8_t Value, const CBitSequence& InSequence)
    {
        TableData[Value] = InSequence;
    }

    const CBitSequence &GetBitSequence(uint8_t Value)
    {
        return TableData[Value];
    }

    template<typename Handler>
    void ForEachSequence(const Handler& InHandler) const
    {
        for (int i = 0; i < UCHAR_MAX+1; ++i)
        {
            if (TableData[i].IsEmpty())
                continue;
            InHandler(i, TableData[i]);
        }
    }

    CSizeType GetEntryCount()
    {
        CSizeType Ret = 0;
        for(int i = 0; i < UCHAR_MAX+1; ++i)
        {
            if(TableData[i].IsEmpty())continue;
            Ret++;
        }
        return Ret;
    }
};

void ConvertToBitSequenceTableImpl(const CHuffmanNode *Root, CBitSequence &CurrentSequence, CBitSequenceTable &OutTable)
{
    if (Root->IsLeaf())
    {
        OutTable.Add(Root->Value, CurrentSequence);
    }
    else
    {
        if (Root->Left)
        {
            CurrentSequence.AddBit(0);
            ConvertToBitSequenceTableImpl(Root->Left, CurrentSequence, OutTable);
            CurrentSequence.PopBit();
        }

        if (Root->Right)
        {
            CurrentSequence.AddBit(1);
            ConvertToBitSequenceTableImpl(Root->Right, CurrentSequence, OutTable);
            CurrentSequence.PopBit();
        }
    }
}

void ConvertToBitSequenceTable(const CHuffmanNode *Root, CBitSequenceTable &OutTable)
{
    CBitSequence Sequence;
    ConvertToBitSequenceTableImpl(Root, Sequence, OutTable);
}

void ConvertToHuffmanTreeImpl(CHuffmanNode *Root, uint8_t InValue, const CBitSequence &Sequence, CSizeType InIndex)
{
    if(!Sequence.IsValidIndex(InIndex))return;
    auto Bit = Sequence[InIndex];
    if(!Bit)
    {
        if(Root->Left)
        {
            CException::Check(Root->Left->EncodedBit == Bit);
        }
        else
        {
            Root->Left = new CHuffmanNode{};
            Root->Left->EncodedBit = Bit;
        }

        if(Sequence.IsLastIndex(InIndex))
        {
            Root->Left->Value = InValue;
        }
        else
        {
            ConvertToHuffmanTreeImpl(Root->Left, InValue, Sequence, InIndex + 1);
        }
    }
    else
    {
        if (Root->Right)
        {
            CException::Check(Root->Right->EncodedBit == Bit);
        }
        else
        {
            Root->Right = new CHuffmanNode{};
            Root->Right->EncodedBit = Bit;
        }

        if(Sequence.IsLastIndex(InIndex))
        {
            Root->Right->Value = InValue;
        }
        else
        {
            ConvertToHuffmanTreeImpl(Root->Right, InValue, Sequence, InIndex + 1);
        }
    }
}

CHuffmanNode* ConvertToHuffmanTree(const CBitSequenceTable &InTable)
{
    CHuffmanNode* Root = new CHuffmanNode{};
    InTable.ForEachSequence([Root](CSizeType Value, const CBitSequence &InSequence) 
    {
        ConvertToHuffmanTreeImpl(Root, Value, InSequence, 0);
    });
    return Root;
}

void CHuffmanCompressor::Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen)
{
    CLog::DebugLog("Start compress...\n");
    CException::Check(SrcLen > 0);
    const uint8_t *SrcBytes = reinterpret_cast<const uint8_t *>(SrcData);
    int Frequency[UCHAR_MAX+1] = {};
    {
        CLog::DebugLog("Initialize frequency...\n");
        CMemory::Fill(Frequency, UCHAR_MAX+1, 0);
        for(int i = 0; i < SrcLen; ++i)Frequency[SrcBytes[i]]++;
    }

    int AlphabetSize = 0;
    CHuffmanNode* InitialNodes[UCHAR_MAX+1] = {};
    {
        CLog::DebugLog("Create initial huffman node...\n");
        for (int i = 0; i < UCHAR_MAX+1; ++i)
        {
            if(Frequency[i] > 0)
            {
                CLog::DebugLog("Frequency %d: %d\n", i, Frequency[i]);
                InitialNodes[AlphabetSize++] = new CHuffmanNode{nullptr, nullptr, i, Frequency[i]};
            }
        }
    }
    if (AlphabetSize <= 0)return;

    CLog::DebugLog("Build huffman node heap...\n");
    CBitSequenceTable Table = {};
    {
        CBinaryHeap<CHuffmanNode *, CHuffmanNodeComparator> NodeHeap(InitialNodes, AlphabetSize);
        CLog::DebugLog("Update huffman node heap...\n");
        for (int i = 0; i < AlphabetSize - 1; ++i)
        {
            CHuffmanNode *Top0 = NodeHeap.Pop();
            CHuffmanNode *Top1 = NodeHeap.Pop();
            CHuffmanNode *NewNode = new CHuffmanNode{Top0, Top1, 0, Top0->Weight + Top1->Weight};
            NodeHeap.Insert(NewNode);
        }
        CHuffmanNode* Root = NodeHeap.Pop();
        CLog::DebugLog("Convert huffman heap to sequence table...\n");
        ConvertToBitSequenceTable(Root, Table);
#ifndef NDEBUG
        {
            Table.ForEachSequence([](CSizeType InIndex, const CBitSequence& Sequence)
            {
                CLog::DebugLog("%d: ");
                for(int i = 0; i < Sequence.Count(); ++i)
                {
                    CLog::DebugLog("%d", Sequence[i]);
                }
                CLog::DebugLog("\n");
            });
        }
#endif
        Root->Delete();
    }

    CLog::DebugLog("Reencode...\n");
    {
        CBitStream Stream;
        for (int i = 0; i < SrcLen; ++i)
        {
#ifndef NDEBUG
            if ( ((SrcLen/100!=0)&& i % (SrcLen / 100) == 0) || i == SrcLen - 1)
            {
                CLog::DebugLog("Compressed: %d/%d, %.3f%%\n", i, SrcLen, i/double(SrcLen) * 100.0);
            }
#endif
            Stream.PutBits(Table.GetBitSequence(SrcBytes[i]));
        }
        CLog::DebugLog("Write compressed data...\n");
        DstLen = Table.GetSerializeSizeInByte() + Stream.GetSerializeSizeInByte();
        DstData = CMemory::Malloc<CByteType>(DstLen);
        CSizeType Step = Table.Serialize(DstData);
        Stream.Serialize(DstData + Step);
    }
}

void CHuffmanCompressor::Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen)
{
    CSizeType Step = 0;

    CLog::DebugLog("Deserialize sequence table...\n");
    CBitSequenceTable Table = {};
    Step += Table.Deserialize(SrcData);
    #ifndef NDEBUG
        {
            Table.ForEachSequence([](CSizeType InIndex, const CBitSequence& Sequence)
            {
                CLog::DebugLog("%d: ");
                for(int i = 0; i < Sequence.Count(); ++i)
                {
                    CLog::DebugLog("%d", Sequence[i]);
                }
                CLog::DebugLog("\n");
            });
        }
#endif

    CLog::DebugLog("Convert sequence table to huffman tree...\n");
    CHuffmanNode* Root = ConvertToHuffmanTree(Table);
    CException::Check(Root);

    CLog::DebugLog("Deserialize compressed stream...\n");
    CBitStream Stream;
    Step += Stream.Deserialize(SrcData + Step);

    CLog::DebugLog("Start decompressing...\n");
    CHuffmanNode *CurNode = Root;
    CArray<CByteType> Bytes = {};
    CSizeType BitCount = Stream.GetBitCount();
    for (int i = 0; i < BitCount; ++i)
    {
#ifndef NDEBUG
        if ( ((BitCount / 100 != 0) && i % (BitCount / 100) == 0) || i == BitCount - 1)
        {
            CLog::DebugLog("Deompressed: %d/%d, %.3f%%\n", i, BitCount, i/double(BitCount) * 100.0);
        }
#endif
        auto NextBit = Stream.NextBit();
        CurNode = CurNode->GetChildByBit(NextBit);
        if (CurNode)
        {
            if (CurNode->IsLeaf())
            {
                Bytes.Add(CurNode->Value);
            }
        }
        else
        {
            CurNode = Root->GetChildByBit(NextBit);
            CException::CheckFormatted(CurNode, "CurNode is nullptr.\n");
            if (CurNode->IsLeaf())
            {
                Bytes.Add(CurNode->Value);
            }
        }
        //auto NextBit = Stream.NextBit();
        //CurNode = CurNode->GetChildByBit(NextBit);
        //if(!CurNode)
        //{
        //    CurNode = Root->GetChildByBit(NextBit);
        //}
        //CException::CheckFormatted(CurNode, "CurNode is nullptr.\n");
        //if(CurNode->IsLeaf())
        //{
        //    Bytes.Add(CurNode->Value);
        //}
    }
    if (Bytes.IsEmpty())return;
    CLog::DebugLog("Write decompressed data...\n");
    DstLen = Bytes.SizeInBytes();
    DstData = CMemory::Malloc<CByteType>(DstLen);
    CMemory::Copy(DstData, Bytes.GetData(), DstLen);
    Root->Delete();
}
