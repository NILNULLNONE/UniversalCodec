#include "gtest/gtest.h"
#include "compressor/HuffmanCompressor.h"
#include "common/File.h"

static const char *TestInFilename = "D:\\Study\\UniversalCodec\\build\\lib\\Debug\\gtestd.lib";
static const char *TestEncodedFilename = "D:\\Study\\UniversalCodec\\build\\lib\\Debug\\gtestd.huff.enc";
static const char *TestDecodedFilename = "D:\\Study\\UniversalCodec\\build\\lib\\Debug\\gtestd.huff.dec";

TEST(CASE_HuffmanCompressor, DISABLED_EncodeDecode)
{
    CHuffmanCompressor Compressor;
    CByteType Bytes[] = { 128, 128, 128, 1};
    CSizeType Len = sizeof(Bytes) / sizeof(CByteType);
    CByteType* OutBytes = nullptr;
    CSizeType OutLen = 0;
    Compressor.Compress(OutBytes, OutLen, Bytes, Len);
    CByteType* OutBytes2 = nullptr;
    CSizeType OutLen2 = 0;
    Compressor.Decompress(OutBytes2, OutLen2, OutBytes, OutLen);
    CMemory::SafeFree(OutBytes);
    CMemory::SafeFree(OutBytes2);
}

TEST(CASE_HuffmanCompressor, EncodeDecode2)
{
    CHuffmanCompressor Compressor;
    CByteType Bytes[] = { 128, 128, 128, 128,
                        100, 100, 100,
                        50, 50, 
                        2
                        };
    CSizeType Len = sizeof(Bytes) / sizeof(CByteType);
    CByteType* OutBytes = nullptr;
    CSizeType OutLen = 0;
    Compressor.Compress(OutBytes, OutLen, Bytes, Len);
    CByteType* OutBytes2 = nullptr;
    CSizeType OutLen2 = 0;
    Compressor.Decompress(OutBytes2, OutLen2, OutBytes, OutLen);
    CMemory::SafeFree(OutBytes);
    CMemory::SafeFree(OutBytes2);
}

TEST(CASE_HuffmanCompressor, DISABLED_EncodeBig)
{
    CByteType* Bytes = nullptr;
    CSizeType Len = 0;
    CFile::LoadDataFromFile(TestInFilename, Bytes, Len);
    
    CHuffmanCompressor Compressor;
    CByteType* CompressedBytes = nullptr;
    CSizeType CompressedLen = 0;
    Compressor.Compress(CompressedBytes, CompressedLen, Bytes, Len);
    CFile::SaveDataToFile(TestEncodedFilename, CompressedBytes, CompressedLen);

    CMemory::SafeFree(Bytes);
    CMemory::SafeFree(CompressedBytes);
}

TEST(CASE_HuffmanCompressor, DISABLED_DecodeBig)
{
    CByteType* Bytes = nullptr;
    CSizeType Len = 0;
    CFile::LoadDataFromFile(TestEncodedFilename, Bytes, Len);

    CHuffmanCompressor Compressor;
    CByteType* DecompressedBytes = nullptr;
    CSizeType DecompressedLen = 0;
    Compressor.Decompress(DecompressedBytes, DecompressedLen, Bytes, Len);
    CFile::SaveDataToFile(TestDecodedFilename, DecompressedBytes, DecompressedLen);

    CMemory::SafeFree(Bytes);
    CMemory::SafeFree(DecompressedBytes);
}