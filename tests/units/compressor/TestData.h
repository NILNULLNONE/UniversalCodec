#pragma once
#include "common/TypeDef.h"
#include "common/Memory.h"
#include "compressor/HuffmanCompressor.h"
#include "compressor/LZ77Compressor.h"
struct CCompressorTestData
{
    CHuffmanCompressor HuffmanCompressor;
    CLZ77Compressor LZ77Compressor;

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

    const char *TextTestInFilename = "D:\\Study\\UniversalCodec\\build\\tests\\units\\INSTALL.vcxproj";
    const char *TextTestEncodedFilename = "D:\\Study\\UniversalCodec\\build\\tests\\units\\INSTALL.vcxproj.enc";
    const char *TextTestDecodedFilename = "D:\\Study\\UniversalCodec\\build\\tests\\units\\INSTALL.vcxproj.dec";

    void Free()
    {
        CMemory::SafeFree(OutData1);
        CMemory::SafeFree(OutData2);
    }
};

extern CCompressorTestData GTestData;