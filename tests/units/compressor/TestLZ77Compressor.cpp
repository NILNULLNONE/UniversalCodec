#include "gtest/gtest.h"
#include "compressor/LZ77Compressor.h"
#include "TestData.h"

TEST(CASE_LZ77Compressor, Compress1)
{
    CLZ77Compressor Compressor;
    Compressor.Compress(GTestData.OutData1, GTestData.OutData1Len, 
        GTestData.InData1, GTestData.InData1Len);
    Compressor.Decompress(GTestData.OutData2, GTestData.OutData2Len,
        GTestData.OutData1, GTestData.OutData1Len);
    GTestData.Free();
}