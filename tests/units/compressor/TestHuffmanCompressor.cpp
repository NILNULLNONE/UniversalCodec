#include "gtest/gtest.h"
#include "common/File.h"
#include "TestData.h"
#include "TestCompressor.h"

TEST(CASE_HuffmanCompressor,EncodeDecode)
{
    CompressorConsistencyCheck(GTestData.HuffmanCompressor, GTestData.InData1, GTestData.InData1Len);
}

TEST(CASE_HuffmanCompressor, EncodeDecode2)
{
    CompressorConsistencyCheck(GTestData.HuffmanCompressor, GTestData.InData2, GTestData.InData2Len);
}

TEST(CASE_HuffmanCompressor, CompressRepeat)
{
    CompressorConsistencyCheck(GTestData.HuffmanCompressor, GTestData.InDataRepeat1024, GTestData.InDataRepeat1024Len);
}

TEST(CASE_HuffmanCompressor, DISABLED_EncodeBig)
{
    EncodeFileImpl(GTestData.HuffmanCompressor, GTestData.TestInFilename_mid, GTestData.TestEncodedFilename_mid);
}

TEST(CASE_HuffmanCompressor, DISABLED_DecodeBig)
{
    DecodeFileImpl(GTestData.HuffmanCompressor, GTestData.TestEncodedFilename_mid, GTestData.TestDecodedFilename_mid);
}

TEST(CASE_HuffmanCompressor, TextEncodeBig)
{
    EncodeFileImpl(GTestData.HuffmanCompressor, GTestData.TextTestInFilename, GTestData.TextTestEncodedFilename);
}

TEST(CASE_HuffmanCompressor,TextDecodeBig)
{
    DecodeFileImpl(GTestData.HuffmanCompressor, GTestData.TextTestEncodedFilename, GTestData.TextTestDecodedFilename);
}