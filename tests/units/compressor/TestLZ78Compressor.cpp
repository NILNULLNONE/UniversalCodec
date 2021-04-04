#include "common/Log.h"
#include "gtest/gtest.h"
#include "compressor/CompressorInterface.h"
#include "TestData.h"
#include "TestCompressor.h"

TEST(CASE_LZ78Compressor, Compress1)
{
    CompressorConsistencyCheck(GTestData.LZ78Compressor, GTestData.InData1, GTestData.InData1Len);
}

TEST(CASE_LZ78Compressor, Compress2)
{
    CompressorConsistencyCheck(GTestData.LZ78Compressor, GTestData.InData2, GTestData.InData2Len);
}

TEST(CASE_LZ78Compressor, CompressRepeat)
{
    CompressorConsistencyCheck(GTestData.LZ78Compressor, GTestData.InDataRepeat1024, GTestData.InDataRepeat1024Len);
}

 TEST(CASE_LZ78Compressor, TextEncodeBig)
 {
     EncodeFileImpl(GTestData.LZ78Compressor, GTestData.TextTestInFilename, GTestData.TextTestEncodedFilename);
 }

 TEST(CASE_LZ78Compressor, TextDecodeBig)
 {
     DecodeFileImpl(GTestData.LZ78Compressor, GTestData.TextTestEncodedFilename, GTestData.TextTestDecodedFilename);
 }

// TEST(CASE_LZ78Compressor, EncodeSmall)
// {
//     EncodeFileImpl(GTestData.LZ78Compressor, GTestData.BinTestFilename_small, GTestData.BinTestEncodedFilename_small);
// }

// TEST(CASE_LZ78Compressor, DecodeSmall)
// {
//     DecodeFileImpl(GTestData.LZ78Compressor, GTestData.BinTestEncodedFilename_small, GTestData.BinTestDecodedFilename_small);
// }

// TEST(CASE_LZ78Compressor, EncodeBig)
// {
//     EncodeFileImpl(GTestData.LZ78Compressor, GTestData.TestInFilename_big, GTestData.TestEncodedFilename_big);
// }

// TEST(CASE_LZ78Compressor, DecodeBig)
// {
//     DecodeFileImpl(GTestData.LZ78Compressor, GTestData.TestEncodedFilename_big, GTestData.TestDecodedFilename_big);
// }