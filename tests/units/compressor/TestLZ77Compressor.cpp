#include "common/Log.h"
#include "gtest/gtest.h"
#include "compressor/CompressorInterface.h"
#include "TestData.h"
#include "TestCompressor.h"

TEST(CASE_LZ77Compressor, Compress1)
{
    CompressorConsistencyCheck(GTestData.LZ77Compressor, GTestData.InData1, GTestData.InData1Len);
}

TEST(CASE_LZ77Compressor, Compress2)
{
    CompressorConsistencyCheck(GTestData.LZ77Compressor, GTestData.InData2, GTestData.InData2Len);
}

TEST(CASE_LZ77Compressor, CompressRepeat)
{
    CompressorConsistencyCheck(GTestData.LZ77Compressor, GTestData.InDataRepeat1024, GTestData.InDataRepeat1024Len);
}

// TEST(CASE_LZ77Compressor, TextEncodeBig)
// {
//     EncodeFileImpl(GTestData.LZ77Compressor, GTestData.TextTestInFilename, GTestData.TextTestEncodedFilename);
// }

// TEST(CASE_LZ77Compressor, TextDecodeBig)
// {
//     DecodeFileImpl(GTestData.LZ77Compressor, GTestData.TextTestEncodedFilename, GTestData.TextTestDecodedFilename);
// }

// TEST(CASE_LZ77Compressor, DISABLED_EncodeBig)
// {
//     EncodeFileImpl(GTestData.LZ77Compressor, GTestData.TestInFilename_big, GTestData.TestEncodedFilename_big);
// }

// TEST(CASE_LZ77Compressor, DISABLED_DecodeBig)
// {
//     DecodeFileImpl(GTestData.LZ77Compressor, GTestData.TestEncodedFilename_big, GTestData.TestDecodedFilename_big);
// }