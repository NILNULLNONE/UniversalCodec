#include "TestCompressor.h"
#include "common/File.h"
#include "TestData.h"
#include "common/Log.h"
#include "gtest/gtest.h"

void CompressorConsistencyCheck(CCompressor &Compressor, CByteType *InData, CSizeType InDatLen)
{
    Compressor.Compress(GTestData.OutData1, GTestData.OutData1Len,
                        InData, InDatLen);
    Compressor.Decompress(GTestData.OutData2, GTestData.OutData2Len,
                          GTestData.OutData1, GTestData.OutData1Len);
    EXPECT_EQ(InDatLen, GTestData.OutData2Len);
    for(int i = 0; i < InDatLen; ++i)
    {
        EXPECT_EQ(InData[i], GTestData.OutData2[i]);
    }
    GTestData.Free();
}

void EncodeFileImpl(CCompressor &Compressor, const char *OriginFN, const char *EncodedFN)
{
    CFile::LoadDataFromFile(OriginFN, 
        GTestData.OutData1, GTestData.OutData1Len);
    Compressor.Compress(GTestData.OutData2, GTestData.OutData2Len, 
        GTestData.OutData1, GTestData.OutData1Len);
    CFile::SaveDataToFile(EncodedFN, 
        GTestData.OutData2, GTestData.OutData2Len);
    CLog::DebugLog("Compressor ratio: %.3f%% (%u / %u)\n",
                   double(GTestData.OutData2Len) / GTestData.OutData1Len * 100.0,
                   GTestData.OutData2Len,
                   GTestData.OutData1Len
                   );
    GTestData.Free();
}

void DecodeFileImpl(CCompressor &Compressor, const char *EncodedFN, const char *DecodedFN)
{
    CFile::LoadDataFromFile(EncodedFN, 
        GTestData.OutData1, GTestData.OutData1Len);
    Compressor.Decompress(GTestData.OutData2, GTestData.OutData2Len, 
        GTestData.OutData1, GTestData.OutData1Len);
    CFile::SaveDataToFile(DecodedFN, 
        GTestData.OutData2, GTestData.OutData2Len);
    GTestData.Free();
}