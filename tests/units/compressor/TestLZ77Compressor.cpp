#include "common/Log.h"
#include "gtest/gtest.h"
#include "compressor/LZ77Compressor.h"
#include "TestData.h"
#include "TestCompressor.h"

TEST(CASE_LZ77Compressor, Compress1)
{
    CLZ77Compressor Compressor;
    Compressor.Compress(GTestData.OutData1, GTestData.OutData1Len, 
        GTestData.InData1, GTestData.InData1Len);
    Compressor.Decompress(GTestData.OutData2, GTestData.OutData2Len,
        GTestData.OutData1, GTestData.OutData1Len);

    CLog::DebugLog("SrcLen: %u\nSrcData: ", GTestData.InData1Len);
    for(int i = 0; i < GTestData.InData1Len; ++i)
    {
        CLog::DebugLog("%u ", (uint32_t)GTestData.InData1[i]);
    }
    CLog::DebugLog("\n");

    CLog::DebugLog("EncodedLen: %u\nEncodedData: ", GTestData.OutData1Len);
    for (int i = 0; i < GTestData.OutData1Len; ++i)
    {
        CLog::DebugLog("%u ", (uint32_t)GTestData.OutData1[i]);
    }
    CLog::DebugLog("\n");

    CLog::DebugLog("DecodedLen: %u\nDecodedData: ", GTestData.OutData2Len);
    for (int i = 0; i < GTestData.OutData2Len; ++i)
    {
        CLog::DebugLog("%u ", (uint32_t)GTestData.OutData2[i]);
    }
    CLog::DebugLog("\n");

    GTestData.Free();
}

TEST(CASE_LZ77Compressor, Compress2)
{
    CLZ77Compressor Compressor;
    Compressor.Compress(GTestData.OutData1, GTestData.OutData1Len,
                        GTestData.InData2, GTestData.InData2Len);
    Compressor.Decompress(GTestData.OutData2, GTestData.OutData2Len,
                          GTestData.OutData1, GTestData.OutData1Len);

    CLog::DebugLog("SrcLen: %u\nSrcData: ", GTestData.InData2Len);
    for (int i = 0; i < GTestData.InData2Len; ++i)
    {
        CLog::DebugLog("%u ", (uint32_t)GTestData.InData2[i]);
    }
    CLog::DebugLog("\n");

    CLog::DebugLog("EncodedLen: %u\nEncodedData: ", GTestData.OutData1Len);
    for (int i = 0; i < GTestData.OutData1Len; ++i)
    {
        CLog::DebugLog("%u ", (uint32_t)GTestData.OutData1[i]);
    }
    CLog::DebugLog("\n");

    CLog::DebugLog("DecodedLen: %u\nDecodedData: ", GTestData.OutData2Len);
    for (int i = 0; i < GTestData.OutData2Len; ++i)
    {
        CLog::DebugLog("%u ", (uint32_t)GTestData.OutData2[i]);
    }
    CLog::DebugLog("\n");

    GTestData.Free();
}

TEST(CASE_LZ77Compressor, TextEncodeBig)
{
    // CLog::TurnOff();
    EncodeFileImpl(GTestData.LZ77Compressor, GTestData.TextTestInFilename, GTestData.TextTestEncodedFilename);
    // CLog::TurnOn();
}

TEST(CASE_LZ77Compressor, TextDecodeBig)
{
    // CLog::TurnOff();
    DecodeFileImpl(GTestData.LZ77Compressor, GTestData.TextTestEncodedFilename, GTestData.TextTestDecodedFilename);
    // CLog::TurnOn();
}

TEST(CASE_LZ77Compressor, DISABLED_EncodeBig)
{
    // CLog::TurnOff();
    EncodeFileImpl(GTestData.LZ77Compressor, GTestData.TestInFilename_big, GTestData.TestEncodedFilename_big);
    // CLog::TurnOn();
}

TEST(CASE_LZ77Compressor, DecodeBig)
{
    // CLog::TurnOff();
    DecodeFileImpl(GTestData.LZ77Compressor, GTestData.TestEncodedFilename_big, GTestData.TestDecodedFilename_big);
    // CLog::TurnOn();
}