#include "gtest/gtest.h"
#include "compressor/HuffmanCompressor.h"
#include "common/File.h"

//static const char *TestInFilename = "D:\\Study\\UniversalCodec\\build\\lib\\Debug\\gtestd.lib";
//static const char *TestEncodedFilename = "D:\\Study\\UniversalCodec\\build\\lib\\Debug\\gtestd.huff.enc";
//static const char *TestDecodedFilename = "D:\\Study\\UniversalCodec\\build\\lib\\Debug\\gtestd.huff.dec";
static const char *TestInFilename = "D:\\Software\\Unreal\\UnrealEngine-release\\Engine\\Binaries\\Win64\\UE4Editor-Renderer.pdb";
static const char *TestEncodedFilename = "D:\\Software\\Unreal\\UnrealEngine-release\\Engine\\Binaries\\Win64\\UE4Editor-Renderer.pdb.enc";
static const char *TestDecodedFilename = "D:\\Software\\Unreal\\UnrealEngine-release\\Engine\\Binaries\\Win64\\UE4Editor-Renderer.pdb.dec";

static const char *TextTestInFilename = "D:\\Study\\UniversalCodec\\build\\tests\\units\\INSTALL.vcxproj";
static const char *TextTestEncodedFilename = "D:\\Study\\UniversalCodec\\build\\tests\\units\\INSTALL.vcxproj.enc";
static const char *TextTestDecodedFilename = "D:\\Study\\UniversalCodec\\build\\tests\\units\\INSTALL.vcxproj.dec";

TEST(CASE_HuffmanCompressor, DISABLED_EncodeDecode)
{
    CHuffmanCompressor Compressor;
    CByteType Bytes[] = { (CByteType)128, (CByteType)128, (CByteType)128, (CByteType)1};
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

TEST(CASE_HuffmanCompressor, DISABLED_EncodeDecode2)
{
    CHuffmanCompressor Compressor;
     CByteType Bytes[] = {(CByteType)128, (CByteType)128, (CByteType)128, (CByteType)128,
                          (CByteType)100, (CByteType)100, (CByteType)100,
                          (CByteType)50, (CByteType)50,
                          (CByteType)2};
    CSizeType Len = sizeof(Bytes) / sizeof(CByteType);
    CByteType* OutBytes = nullptr;
    CSizeType OutLen = 0;
    Compressor.Compress(OutBytes, OutLen, Bytes, Len);
    CByteType* OutBytes2 = nullptr;
    CSizeType OutLen2 = 0;
    Compressor.Decompress(OutBytes2, OutLen2, OutBytes, OutLen);
    EXPECT_EQ(Len, OutLen2);
    for(int i = 0; i < Len; ++i)
    {
        printf("%d/%d,", Bytes[i], OutBytes2[i]);
    }
    printf("\n");
    CMemory::SafeFree(OutBytes);
    CMemory::SafeFree(OutBytes2);
}

TEST(CASE_HuffmanCompressor, EncodeBig)
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

TEST(CASE_HuffmanCompressor, DecodeBig)
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


TEST(CASE_HuffmanCompressor, DISABLED_TextEncodeBig)
{
    CByteType* Bytes = nullptr;
    CSizeType Len = 0;
    CFile::LoadDataFromFile(TextTestInFilename, Bytes, Len);
    
    CHuffmanCompressor Compressor;
    CByteType* CompressedBytes = nullptr;
    CSizeType CompressedLen = 0;
    Compressor.Compress(CompressedBytes, CompressedLen, Bytes, Len);
    CFile::SaveDataToFile(TextTestEncodedFilename, CompressedBytes, CompressedLen);

    CMemory::SafeFree(Bytes);
    CMemory::SafeFree(CompressedBytes);
}

TEST(CASE_HuffmanCompressor, DISABLED_TextDecodeBig)
{
    CByteType* Bytes = nullptr;
    CSizeType Len = 0;
    CFile::LoadDataFromFile(TextTestEncodedFilename, Bytes, Len);

    CHuffmanCompressor Compressor;
    CByteType* DecompressedBytes = nullptr;
    CSizeType DecompressedLen = 0;
    Compressor.Decompress(DecompressedBytes, DecompressedLen, Bytes, Len);
    CFile::SaveDataToFile(TextTestDecodedFilename, DecompressedBytes, DecompressedLen);

    CMemory::SafeFree(Bytes);
    CMemory::SafeFree(DecompressedBytes);
}