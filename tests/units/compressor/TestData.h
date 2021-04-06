#pragma once
#include "common/TypeDef.h"
#include "common/Memory.h"
#include "compressor/CompressorInterface.h"
#include "common/String.h"
#include "common/File.h"

struct CCompressorTestData
{
    // constant
    CHuffmanCompressor HuffmanCompressor;
    CLZ77Compressor LZ77Compressor;
    CRLECompressor RLECompressor;
    CLZ78Compressor LZ78Compressor;

    CByteType InData1[4] = {(CByteType)128, (CByteType)128, (CByteType)128, (CByteType)1};
    CSizeType InData1Len = sizeof(InData1) / sizeof(CByteType);
    CByteType InData2[10] = {(CByteType)128, (CByteType)128, (CByteType)128, (CByteType)128,
                             (CByteType)100, (CByteType)100, (CByteType)100,
                             (CByteType)50, (CByteType)50,
                             (CByteType)2};
    CSizeType InData2Len = sizeof(InData2) / sizeof(CByteType);

    CByteType *OutData1 = nullptr;
    CSizeType OutData1Len = 0;
    CByteType *OutData2 = nullptr;
    CSizeType OutData2Len = 0;

    const CString TestDataDir = "..\\tests\\testdata";
    const CString TestMediaDir = CFilePath::Join(TestDataDir, "media");
    const CString TestCompressorDir = CFilePath::Join(TestDataDir, "compressor");

    const char *BinFN1KB = CFilePath::Join(TestCompressorDir, "Bin_1KB.txt").Copy();
    const char *BinFN1KBCompressed = CFilePath::Join(TestCompressorDir, "Bin_1KB.enc").Copy();
    const char *BinFN1KBDeompressed = CFilePath::Join(TestCompressorDir, "Bin_1KB.dec").Copy();

    const char *BinFN1MB = CFilePath::Join(TestCompressorDir, "Bin_1MB.txt").Copy();
    const char *BinFN1MBCompressed = CFilePath::Join(TestCompressorDir, "Bin_1MB.enc").Copy();
    const char *BinFN1MBDeompressed = CFilePath::Join(TestCompressorDir, "Bin_1MB.dec").Copy();

    const char *TextFN1KB = CFilePath::Join(TestCompressorDir, "Text_1KB.txt").Copy();
    const char *TextFN1KBCompressed = CFilePath::Join(TestCompressorDir, "Text_1KB.enc").Copy();
    const char *TextFN1KBDeompressed = CFilePath::Join(TestCompressorDir, "Text_1KB.dec").Copy();

    const char *TextFN1MB = CFilePath::Join(TestCompressorDir, "Text_1MB.txt").Copy();
    const char *TextFN1MBCompressed = CFilePath::Join(TestCompressorDir, "Text_1MB.enc").Copy();
    const char *TextFN1MBDeompressed = CFilePath::Join(TestCompressorDir, "Text_1MB.dec").Copy();

    const char* PNGFN0 = CFilePath::Join(TestMediaDir, "mario.png").Copy();
    const char* PNGFN1 = CFilePath::Join(TestMediaDir, "mushroom3.png").Copy();

    // non-constant
    CByteType *InDataRepeat1024 = nullptr;
    CSizeType InDataRepeat1024Len = 4;

    CCompressorTestData()
    {
        {
            InDataRepeat1024 = CMemory::Malloc<CByteType>(sizeof(CByteType) * InDataRepeat1024Len);
            for(int i = 0; i < InDataRepeat1024Len; ++i)InDataRepeat1024[i] = 17;
        }
    }

    void Free()
    {
        CMemory::SafeFree(OutData1);
        CMemory::SafeFree(OutData2);
    }

    ~CCompressorTestData()
    {
        CMemory::SafeFree(InDataRepeat1024);
        // TODO: free const char*
    }
};

extern CCompressorTestData GTestData;