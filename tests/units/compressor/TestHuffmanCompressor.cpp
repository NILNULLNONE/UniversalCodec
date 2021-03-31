#include "gtest/gtest.h"
#include "compressor/HuffmanCompressor.h"
#include "common/File.h"
#include "TestData.h"
#include "TestCompressor.h"

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

TEST(CASE_HuffmanCompressor, DISABLED_EncodeBig)
{
    EncodeFileImpl(GTestData.HuffmanCompressor, GTestData.TestInFilename_mid, GTestData.TestEncodedFilename_mid);
}

TEST(CASE_HuffmanCompressor, DISABLED_DecodeBig)
{
    DecodeFileImpl(GTestData.HuffmanCompressor, GTestData.TestEncodedFilename_mid, GTestData.TestDecodedFilename_mid);
}

TEST(CASE_HuffmanCompressor, DISABLED_TextEncodeBig)
{
    EncodeFileImpl(GTestData.HuffmanCompressor, GTestData.TextTestInFilename, GTestData.TextTestEncodedFilename);
}

TEST(CASE_HuffmanCompressor, DISABLED_TextDecodeBig)
{
    DecodeFileImpl(GTestData.HuffmanCompressor, GTestData.TextTestEncodedFilename, GTestData.TextTestDecodedFilename);
}