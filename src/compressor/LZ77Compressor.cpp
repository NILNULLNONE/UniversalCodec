#include "LZ77Compressor.h"
#include "common/Array.h"
#include "common/Log.h"

struct CSearchInput
{
    const CByteType* Data;
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
    // CLog::DebugLog("Searching <%u, %u, %u>...\n", Input.SearchStart, Input.IterStart, Input.LookAheadBufferSize);
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
        // CLog::DebugLog("Search Outer: %d / %d\n", Idx, Input.IterStart);
        CSizeType Idx2 = Idx, Iter = Input.IterStart;
        CSizeType IterEnd = Input.IterStart + Input.LookAheadBufferSize;
        while (/* Idx2 < Input.IterStart &&*/ Iter < IterEnd && Iter < Input.DataLen - 1)
        {
            // CLog::DebugLog("Search Inner: %d / %d / %d\n", Idx2, IterEnd, Input.DataLen - 1);
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
    // CLog::DebugLog("Create searching result.\n");
    Result.Dist = MaxInfo.MaxDist;
    Result.Len = MaxInfo.MaxLen;
    Result.EndByte = Input.Data[Input.IterStart + Result.Len];
    // CLog::DebugLog("Searching end: <%u, %u, %u>.\n", Result.Dist, Result.Len, Result.EndByte);
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
    // CLog::DebugLog("Start LZ77 Compressing...\n");
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
#ifndef NDEBUG
        if ((SrcLen / 100 != 0 && Iter % (SrcLen / 100) == 0) || Iter == SrcLen - 1)
            CLog::DebugLog("Compressed: %d/%d, %.3f%%\n", Iter, SrcLen, Iter / double(SrcLen) * 100.0);
#endif
        SearchInput.Data = SrcData;
        SearchInput.DataLen = SrcLen;
        SearchInput.IterStart = Iter;
        SearchInput.LookAheadBufferSize = LookAheadBufferSize;
        SearchInput.SearchStart = Iter ==  SrcLen - 1? Iter : 
                                Iter > SearchBufferSize ? Iter - SearchBufferSize 
                                : 0;
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
    // CLog::DebugLog("Write Decompressed Data: <%u, %u, %u>\n", Dist, Len, EndByte);
    // const CByteType* Start = DecompressedData.End() - Dist;
    for(int Idx = 0; Idx < Len; ++Idx)
    {
        CByteType Byte = *(DecompressedData.End() - Dist);
        DecompressedData.Add( Byte );
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
    CSizeType CompressedDataLen = CompressedData.SizeInBytes();
    while (Iter < CompressedDataLen)
    {
#ifndef NDEBUG
        if ((CompressedDataLen / 100 != 0 && Iter % (CompressedDataLen / 100) == 0) || Iter == CompressedDataLen - 1)
            CLog::DebugLog("Compressed: %d/%d, %.3f%%\n", Iter, CompressedDataLen, Iter / double(CompressedDataLen) * 100.0);
#endif
        Iter += WriteDecompressedData(DecompressedData,
            CompressedData.GetData(), CompressedData.SizeInBytes(), Iter);
    }
    DstLen = DecompressedData.SizeInBytes();
    DstData = CMemory::Malloc<CByteType>(DstLen);
    CMemory::Copy(DstData, DecompressedData.GetData(), DstLen);
}
