#include "gtest/gtest.h"
#include "compressor/HuffmanCompressor.h"
#include "common/File.h"
#include "TestData.h"

TEST(CASE_HuffmanCompressor,EncodeDecode)
{
    CHuffmanCompressor Compressor;
    Compressor.Compress(GTestData.OutData1, GTestData.OutData1Len, 
        GTestData.InData1, GTestData.InData1Len);
    Compressor.Decompress(GTestData.OutData2, GTestData.OutData2Len,
         GTestData.OutData1, GTestData.OutData1Len);
    GTestData.Free();
}

TEST(CASE_HuffmanCompressor, EncodeDecode2)
{
    CHuffmanCompressor Compressor;
    Compressor.Compress(GTestData.OutData1, GTestData.OutData1Len, 
        GTestData.InData2, GTestData.InData2Len);
    Compressor.Decompress(GTestData.OutData2, GTestData.OutData2Len, 
        GTestData.OutData1, GTestData.OutData1Len);
    EXPECT_EQ(GTestData.InData2Len, GTestData.OutData2Len);
    for (int i = 0; i < GTestData.InData2Len; ++i)
    {
        printf("%d/%d,", GTestData.InData2[i], GTestData.OutData2[i]);
    }
    printf("\n");
    GTestData.Free();
}

static void EncodeFileImpl(const char* OriginFN, const char* EncodedFN)
{
    CFile::LoadDataFromFile(OriginFN, 
        GTestData.OutData1, GTestData.OutData1Len);
    CHuffmanCompressor Compressor;
    Compressor.Compress(GTestData.OutData2, GTestData.OutData2Len, 
        GTestData.OutData1, GTestData.OutData1Len);
    CFile::SaveDataToFile(EncodedFN, 
        GTestData.OutData2, GTestData.OutData2Len);
    GTestData.Free();
}

static void DecodeFileImpl(const char* EncodedFN, const char* DecodedFN)
{
    CFile::LoadDataFromFile(EncodedFN, 
        GTestData.OutData1, GTestData.OutData1Len);
    CHuffmanCompressor Compressor;
    Compressor.Decompress(GTestData.OutData2, GTestData.OutData2Len, 
        GTestData.OutData1, GTestData.OutData1Len);
    CFile::SaveDataToFile(DecodedFN, 
        GTestData.OutData2, GTestData.OutData2Len);
    GTestData.Free();
}


TEST(CASE_HuffmanCompressor, DISABLED_EncodeBig)
{
    EncodeFileImpl(GTestData.TestInFilename_mid, GTestData.TestEncodedFilename_mid);
}

TEST(CASE_HuffmanCompressor, DISABLED_DecodeBig)
{
    DecodeFileImpl(GTestData.TestEncodedFilename_mid, GTestData.TestDecodedFilename_mid);
}

TEST(CASE_HuffmanCompressor, DISABLED_TextEncodeBig)
{
    EncodeFileImpl(GTestData.TextTestInFilename, GTestData.TextTestEncodedFilename);
}

TEST(CASE_HuffmanCompressor, DISABLED_TextDecodeBig)
{
    DecodeFileImpl(GTestData.TextTestEncodedFilename, GTestData.TextTestDecodedFilename);
}