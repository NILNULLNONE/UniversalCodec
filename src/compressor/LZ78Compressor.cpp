#include "CompressorInterface.h"
#include "common/Array.h"

// struct CLZ78BinaryTree
// {

// };

using CIndexType = uint16_t;
using CByteArray = CArray<CByteType>;

struct CLZ78Trie
{
private:
    struct CNode
    {
        CNode** Children = nullptr;
        CIndexType Index;
        void PackIndexAndCount(CIndexType InIndex, CSizeType InCnt,
            CIndexType& OutIndex, CSizeType& OutCnt)
        {
            CException::Check(InCnt <= (1<<8));
            CException::Check(InIndex < (1 << 15));
            OutIndex = (InIndex << 1) | (InCnt & 1);
            OutCnt = (InCnt >> 1);
        }

        void UnpackIndexAndCount(CIndexType InIndex, CSizeType InCnt, CIndexType& OutIndex, CSizeType& OutCnt)
        {
            OutCnt = (InCnt << 1) | (InIndex & 1);
            OutIndex = (InIndex >> 1);
        }

        CSizeType GetSerializeSize()
        {
            CSizeType Size = 0;
            Size += sizeof(CIndexType);
            Size += sizeof(CByteType);
            if (!Children)return Size;
            for (int i = 0; i < 256; ++i)
            {
                if (!Children[i])
                    continue;
                Size += sizeof(CByteType);
                Size += Children[i]->GetSerializeSize();
            }
            return Size;
        }

        CSizeType Serialize(CByteType* DstData)
        {
            CSizeType Stride = 0;
            CSizeType ChildCnt = GetChildCnt();
            CIndexType PackedIndex = 0;
            CSizeType PackedCnt = 0;
            PackIndexAndCount(Index, ChildCnt, PackedIndex, PackedCnt);
            Stride += CMemory::Write<CIndexType>(DstData, PackedIndex);
            Stride += CMemory::Write<CByteType>(DstData + Stride, PackedCnt);
            if(ChildCnt <= 0)return Stride;
            for (int i = 0; i < 256; ++i)
            {
                if(!Children[i])continue;
                Stride += CMemory::Write<CByteType>(DstData + Stride, (CByteType)i);
                Stride += Children[i]->Serialize(DstData + Stride);
            }
            return Stride;
        }

        CSizeType Deserialize(const CByteType* SrcData)
        {
            CSizeType Stride = 0;
            CSizeType ChildCnt = 0;
            Stride += CMemory::Read<CIndexType>(SrcData, Index);
            CByteType Byte = 0;
            Stride += CMemory::Read<CByteType>(SrcData + Stride, Byte);
            UnpackIndexAndCount(Index, (CSizeType)Byte, Index, ChildCnt);
            if(ChildCnt > 0)
            {
                if(!Children)
                {
                    Children = new CNode*[256];
                    CMemory::Zero(Children, sizeof(CNode*) * 256);
                }
                CByteType ChildIdx = 0;
                for(int Idx = 0; Idx < ChildCnt; ++Idx)
                {
                    Stride += CMemory::Read<CByteType>(SrcData + Stride, ChildIdx);
                    if (!Children[ChildIdx])
                    {
                        Children[ChildIdx] = new CNode{nullptr, 0};
                    }
                    Stride += Children[ChildIdx]->Deserialize(SrcData + Stride);
                }
            }
            return Stride;
        }

        CSizeType GetChildCnt()
        {
            if(!Children)return 0;
            CSizeType Cnt = 0;
            for(int i = 0; i < 256; ++i)
            {
                if(Children[i])
                {
                    ++Cnt;
                }
            }
            return Cnt;
        }

        void Build(CArray<CByteArray>& Table, CByteArray& Seq)
        {
            if(!Children)return;
            for(int i = 0; i < 256; ++i)
            {
                if(Children[i])
                {
                    Seq.Add((CByteType)i);
                    if(Table.Count() <= Children[i]->Index)
                    {
                        Table.Resize(Children[i]->Index+1);
                    }
                    Table[Children[i]->Index] = Seq;
                    Children[i]->Build(Table, Seq);
                    Seq.RemoveLast();
                }
            }
        }

#ifndef NDEBUG
        void PrintImpl(int Depth)
        {
            for (int i = 0; i < Depth; ++i)CLog::DebugLog("\t");
            CLog::DebugLog("<%hu>\n", Index);
            if (!Children)return;
            for (int i = 0; i < 256; ++i)
            {
                if (!Children[i])continue;
                for (int j = 0; j < Depth; ++j)
                    CLog::DebugLog("\t");
                CLog::DebugLog("[%d]:\n", i);
                Children[i]->PrintImpl(Depth+1);
            }
        }

        void Print()
        {
            PrintImpl(0);
        }
#endif
    } *Root, *CurrentNode;

    CIndexType CurrentIndex;
    CArray<CByteArray> Table;
    const int MaxChildren = 256;
    const CSizeType MaxNodes = 2048;
public:
    CSizeType NodeMem = 0;
    CSizeType NodeNum = 0;
    CLZ78Trie()
    {
        Root = new CNode{ nullptr, 0 };
        CurrentNode = Root;
        CurrentIndex = 0;
        // Root->Children = new CNode *[MaxChildren];
        // Root->Index = 0;
        // CurrentNode = Root;
        // CurrentIndex = 1;
        // for (int i = 0; i < MaxChildren; ++i)
        // {
        //     Root->Children[i] = new CNode{nullptr, CurrentIndex++};
        // }
    }

    bool HasChild(const CByteType &Byte)
    {
        return CurrentNode && CurrentNode->Children && CurrentNode->Children[Byte];
    }

    CSizeType GetSerializeSize()
    {
        return Root->GetSerializeSize();
    }

    CSizeType Serialize(CByteType *DstData)
    {
        return Root->Serialize(DstData);
    }

    CSizeType Deserialize(const CByteType* SrcData)
    {
        return Root->Deserialize(SrcData);
    }

    CIndexType Insert(const CByteType &Byte)
    {
        CException::Check(CurrentNode != nullptr);
        CIndexType RetIndex = CurrentNode->Index;//CurrentIndex;
        if(!CurrentNode->Children)
        {
            CurrentNode->Children = new CNode *[MaxChildren];
            CMemory::Zero(CurrentNode->Children, sizeof(CNode*) * MaxChildren);
        }

        if (!CurrentNode->Children[Byte])
        {
            CurrentNode->Children[Byte] = new CNode{nullptr, ++CurrentIndex};
        }

        NodeNum++;
        NodeMem += sizeof(CNode*) * MaxChildren + sizeof(CNode) * MaxChildren;
        return RetIndex;
    }

    CIndexType GetCurrentIndex()
    {
        return CurrentNode->Index;
    }

    bool CanInsert() const
    {
        const CSizeType MaxMem = 8 * 1024 * 1024;
        return NodeNum < MaxNodes && NodeMem < MaxMem;
    }

    void Next(CByteType Byte)
    {
        CException::Check(CurrentNode != nullptr 
            &&CurrentNode->Children != nullptr 
            && CurrentNode->Children[Byte] != nullptr);
        CurrentNode = CurrentNode->Children[Byte];
    }

    void Reset()
    {
        CurrentNode = Root;
    }

    void BuildTable()
    {
        CByteArray Tmp = {};
        Root->Build(Table, Tmp);
    }

    const CByteArray& GetTableEntry(CIndexType InIndex)
    {
        return Table[InIndex];
    }

#ifndef NDEBUG
    void Print()const
    {
        Root->Print();
    }
#endif
};

struct CLZ78Packet
{
    CIndexType Index;
    CByteType Byte;
};

void WriteToCompressedData(CArray<CLZ78Packet> &DstData, CIndexType Index, CByteType Byte)
{
    // DstData.AddElements({(CByteType)(Index&( (1<<8)-1)), (CByteType)( (Index& ( ( (1<<8)-1 ) << 8 ) ) >> 8), Byte});
    
    // DstData.AddElements({0, 0, 0});
    // CMemory::Write<CIndexType>(DstData.End() - sizeof(CIndexType) - sizeof(CByteType), Index);
    // CMemory::Write<CByteType>(DstData.End() - sizeof(CByteType), Byte);

    DstData.Add({Index, Byte});
}

void CLZ78Compressor::Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen)
{
    CArray<CLZ78Packet> Result = {};
    CSizeType Iter = 0;
    CLZ78Trie Trie = {};
    while(Iter < SrcLen)
    {
        CByteType Byte = SrcData[Iter];
        if (Iter == SrcLen - 1)
        {
            WriteToCompressedData(Result, Trie.GetCurrentIndex(), Byte);
            break;
        }
        if (!Trie.HasChild(Byte))
        {
            CIndexType Index = 0;
            if(Trie.CanInsert())
            {
                Index = Trie.Insert(Byte);
            }
            else
            {
                Index = Trie.GetCurrentIndex();
            }

            WriteToCompressedData(Result, Index, Byte);
            Trie.Reset();
        }
        else
        {
            Trie.Next(Byte);
        }
        ++Iter;
    }
    Trie.Reset();
    auto TrieSize = Trie.GetSerializeSize();
    DstLen = TrieSize + Result.GetSerializeSizeInByte();
    DstData = CMemory::Malloc<CByteType>(DstLen);
    Trie.Serialize(DstData);
    Result.Serialize(DstData + TrieSize);
#ifndef NDEBUG
     //Trie.Print();
     //for(int i = 0; i < TrieSize; ++i)
     //{
     //   CLog::DebugLog("%u ", (uint32_t)DstData[i]);    
     //}
     //CLog::DebugLog("\n");
     //for(int i = 0; i < Result.Count(); ++i)
     //{
     //    CLog::DebugLog("%u,%u ", (uint32_t)Result[i].Index, (uint32_t)Result[i].Byte);
     //}
     //CLog::DebugLog("\n");
    CLog::DebugLog("Trie: %u\n", TrieSize);
    CLog::DebugLog("Payload: %u\n", Result.GetSerializeSizeInByte());
    CLog::DebugLog("NodeNum: %u\n", Trie.NodeNum);
    CLog::DebugLog("NodeMem: %u\n", Trie.NodeMem);
#endif
}

void WriteDecompressData(CByteArray &OutData, const CByteArray &InEntry, CByteType EndByte)
{
    OutData.Concat(InEntry);
    OutData.Add(EndByte);
}

void CLZ78Compressor::Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen)
{
    CLZ78Trie Trie;
    CArray<CLZ78Packet> Payload;
    auto Stride = Trie.Deserialize(SrcData);
    Payload.Deserialize(SrcData + Stride);
    Trie.BuildTable();
    CByteArray UncompressedData = {};
    for(CSizeType Idx = 0; Idx < Payload.Count(); ++Idx)
    {
        const auto& Packet = Payload.At(Idx);
        if(Packet.Index == 0)
        {
            UncompressedData.Add(Packet.Byte);
            continue;
        }
        const auto& Entry = Trie.GetTableEntry(Packet.Index);
        WriteDecompressData(UncompressedData, Entry, Packet.Byte);
    }
    UncompressedData.DiscardTo(DstData, DstLen);
}