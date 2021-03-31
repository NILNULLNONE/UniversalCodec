#include "TestCompressor.h"
#include "common/File.h"
#include "TestData.h"

void EncodeFileImpl(CCompressor &Compressor, const char *OriginFN, const char *EncodedFN)
{
    CFile::LoadDataFromFile(OriginFN, 
        GTestData.OutData1, GTestData.OutData1Len);
    Compressor.Compress(GTestData.OutData2, GTestData.OutData2Len, 
        GTestData.OutData1, GTestData.OutData1Len);
    CFile::SaveDataToFile(EncodedFN, 
        GTestData.OutData2, GTestData.OutData2Len);
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