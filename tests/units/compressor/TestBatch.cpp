#include "TestCompressor.h"
#include "gtest/gtest.h"
#include "common/Array.h"
#include "TestData.h"

// TEST(CASE_CompressorBatch, All)
// {
//     CArray<CCompressor*> Compressors = {};
//     Compressors.Add(new CHuffmanCompressor);
//     // Compressors.Add(new CLZ77Compressor);
//     // Compressors.Add(new CRLECompressor);
//     for(int Idx = 0; Idx < Compressors.Count(); ++Idx) 
//     {
//         auto& Compressor = *Compressors[Idx];
//         CompressorConsistencyCheck(Compressor, GTestData.InData1, GTestData.InData1Len);
//         CompressorConsistencyCheck(Compressor, GTestData.InData2, GTestData.InData2Len);
//         CompressorConsistencyCheck(Compressor, GTestData.InDataRepeat1024, GTestData.InDataRepeat1024Len);
//         // EncodeFileImpl(Compressor, GTestData.TextTestInFilename, GTestData.TextTestEncodedFilename);
//         // DecodeFileImpl(Compressor, GTestData.TextTestEncodedFilename, GTestData.TextTestDecodedFilename);
//     }

//     for(int Idx = 0; Idx < Compressors.Count(); ++Idx)
//     {
//         delete Compressors[Idx];
//     }
// }