#include "common/Log.h"
#include "gtest/gtest.h"
#include "compressor/CompressorInterface.h"
#include "TestData.h"
#include "TestCompressor.h"

TEST(CASE_RLECompressor, Compress1)
{
    CompressorConsistencyCheck(GTestData.RLECompressor, GTestData.InData1, GTestData.InData1Len);
}

TEST(CASE_RLECompressor, Compress2)
{
    CompressorConsistencyCheck(GTestData.RLECompressor, GTestData.InData2, GTestData.InData2Len);
}

TEST(CASE_RLECompressor, CompressRepeat)
{
    CompressorConsistencyCheck(GTestData.RLECompressor, GTestData.InDataRepeat1024, GTestData.InDataRepeat1024Len);
}

TEST(CASE_RLECompressor, TextEncodeBig)
{
    EncodeFileImpl(GTestData.RLECompressor, GTestData.TextTestInFilename, GTestData.TextTestEncodedFilename);
}

TEST(CASE_RLECompressor, TextDecodeBig)
{
    DecodeFileImpl(GTestData.RLECompressor, GTestData.TextTestEncodedFilename, GTestData.TextTestDecodedFilename);
}

TEST(CASE_RLECompressor, DISABLED_EncodeBig)
{
    EncodeFileImpl(GTestData.RLECompressor, GTestData.TestInFilename_big, GTestData.TestEncodedFilename_big);
}

TEST(CASE_RLECompressor, DISABLED_DecodeBig)
{
    DecodeFileImpl(GTestData.RLECompressor, GTestData.TestEncodedFilename_big, GTestData.TestDecodedFilename_big);
}