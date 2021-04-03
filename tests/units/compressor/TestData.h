#pragma once
#include "common/TypeDef.h"
#include "common/Memory.h"
#include "compressor/CompressorInterface.h"

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

    const char *TestInFilename_mid = "D:\\Study\\UniversalCodec\\build\\bin\\Debug\\gtestd.lib";
    const char *TestEncodedFilename_mid = "D:\\Study\\UniversalCodec\\build\\bin\\Debug\\gtestd.huff.enc";
    const char *TestDecodedFilename_mid = "D:\\Study\\UniversalCodec\\build\\bin\\Debug\\gtestd.huff.dec";

    const char *TestInFilename_big = "D:\\Software\\Unreal\\UnrealEngine-release\\Engine\\Binaries\\Win64\\UE4Editor-Renderer.pdb";
    const char *TestEncodedFilename_big = "D:\\Software\\Unreal\\UnrealEngine-release\\Engine\\Binaries\\Win64\\UE4Editor-Renderer.pdb.enc";
    const char *TestDecodedFilename_big = "D:\\Software\\Unreal\\UnrealEngine-release\\Engine\\Binaries\\Win64\\UE4Editor-Renderer.pdb.dec";

    const char *BinTestFilename_small = "D:\\Software\\Unreal\\UnrealEngine-release\\Engine\\Binaries\\Win64\\UE4Editor-PortalMessages.pdb";
    const char *BinTestEncodedFilename_small = "D:\\Software\\Unreal\\UnrealEngine-release\\Engine\\Binaries\\Win64\\UE4Editor-PortalMessages.pdb.enc";
    const char *BinTestDecodedFilename_small = "D:\\Software\\Unreal\\UnrealEngine-release\\Engine\\Binaries\\Win64\\UE4Editor-PortalMessages.pdb.dec";

    // const char *TextTestInFilename = "D:\\Study\\UniversalCodec\\build\\tests\\units\\INSTALL.vcxproj";
    // const char *TextTestEncodedFilename = "D:\\Study\\UniversalCodec\\build\\tests\\units\\INSTALL.vcxproj.enc";
    // const char *TextTestDecodedFilename = "D:\\Study\\UniversalCodec\\build\\tests\\units\\INSTALL.vcxproj.dec";

    const char *TextTestInFilename = "C:\\Users\\35974\\Downloads\\1617460103.txt";
    const char *TextTestEncodedFilename = "C:\\Users\\35974\\Downloads\\1617460103.txt.enc";
    const char *TextTestDecodedFilename = "C:\\Users\\35974\\Downloads\\1617460103.txt.dec";

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
    }
};

extern CCompressorTestData GTestData;