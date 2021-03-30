#include "LZ77Compressor.h"
#include "common/Array.h"
#include "common/Log.h"

struct CSearchInput
{
    CByteType* Data;
    CSizeType DataLen;
    CSizeType SearchStart;
    CSizeType IterStart;
    CSizeType LookAheadBufferSize;
};

struct CSearchResult
{
    CSizeType Dist;
    CSizeType Len;
    CByteType EndByte;
};

static void Search(const CSearchInput& Input, CSearchResult& Result)
{
    struct CMaxInfo
    {
        CSizeType MaxLen;
        CSizeType MaxDist;
        // CSizeType Index;
    };

    CMaxInfo MaxInfo;
    MaxInfo.MaxLen = 0;
    MaxInfo.MaxDist = Input.IterStart - Input.SearchStart;
    // MaxInfo.Index = Input.SearchStart;
    for (CSizeType Idx = Input.SearchStart; Idx < Input.IterStart; ++Idx)
    {
        CSizeType Idx2 = Idx, Iter = Input.IterStart;
        CSizeType IterEnd = Input.IterStart + Input.LookAheadBufferSize;
        while (Idx2 < Input.IterStart && Iter < IterEnd && Iter < Input.DataLen - 1)
        {
            if(Input.Data[Idx2] == Input.Data[Iter])
            {
                Idx2++;
                Iter++;
            }
            else
            {
                break;
            }
        }
        CSizeType MatchLen = Iter - Input.IterStart;
        if(MatchLen > MaxInfo.MaxLen)
        {
            MaxInfo.MaxLen = MatchLen;
            MaxInfo.MaxDist = (Input.IterStart - Idx);
            // MaxInfo.Index = Idx;
        }
    }
    Result.Dist = MaxInfo.MaxDist;
    Result.Len = MaxInfo.MaxLen;
    Result.EndByte = Input.Data[Input.IterStart + Result.Len];
}

static void WriteSearchData(CArray<CByteType>& CompressedData, const CSearchResult& Result)
{
    CompressedData.AddElements(
    { 
        static_cast<CByteType>(Result.Dist),
        static_cast<CByteType>(Result.Len),
        static_cast<CByteType>(Result.EndByte)
    });
}

void CLZ77Compressor::Compress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen)
{
    CLog::DebugLog("Start LZ77 Compressing...\n");
    DstData = nullptr;
    DstLen = 0;
    CArray<CByteType> CompressedData = {};
    const CSizeType LookAheadBufferSize = 255;
    const CSizeType SearchBufferSize = 255;
    CSizeType Iter = 0, SearchIter = 0;
    CSearchInput SearchInput;
    CSearchResult SearchResult;
    while(Iter < SrcLen)
    {
        CLog::DebugLog("Iter / SrcLen : %u / %u\n", Iter, SrcLen);
        SearchInput.Data = DstData;
        SearchInput.DataLen = DstLen;
        SearchInput.IterStart = Iter;
        SearchInput.LookAheadBufferSize = LookAheadBufferSize;
        SearchInput.SearchStart = CMath::Max(0u, Iter - SearchBufferSize);
        Search(SearchInput, SearchResult);
        WriteSearchData(CompressedData, SearchResult);
        Iter += SearchResult.Len + 1;
    }
    DstLen = CompressedData.GetSerializeSizeInByte();
    DstData = CMemory::Malloc<CByteType>(DstLen);
    CompressedData.Serialize(DstData);
}

static CSizeType WriteDecompressedData(CArray<CByteType>& DecompressedData,
    const CByteType* Data, CSizeType DataLen, CSizeType Iter)
{
    CSizeType Dist = Data[Iter];
    CSizeType Len = Data[Iter+1];
    CSizeType EndByte = Data[Iter+2];
    const CByteType* Start = DecompressedData.End() - Dist;
    for(int Idx = 0; Idx < Len; ++Idx)
    {
        DecompressedData.Add(Start[Idx]);
    }
    DecompressedData.Add(EndByte);
    return 3;
}

void CLZ77Compressor::Decompress(CByteType *&DstData, CSizeType &DstLen, const CByteType *SrcData, const CSizeType SrcLen)
{
    CArray<CByteType> DecompressedData = {};
    CArray<CByteType> CompressedData = {};
    CompressedData.Deserialize(SrcData);
    CSizeType Iter = 0;
    while(Iter < SrcLen)
    {
        Iter += WriteDecompressedData(DecompressedData,
            SrcData, SrcLen, Iter);
    }
    DstLen = DecompressedData.SizeInBytes();
    DstData = CMemory::Malloc<CByteType>(DstLen);
    CMemory::Copy(DstData, DecompressedData.GetData(), DstLen);
}
