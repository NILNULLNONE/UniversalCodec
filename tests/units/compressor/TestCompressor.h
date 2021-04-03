#pragma once
#include "compressor/CompressorInterface.h"

void CompressorConsistencyCheck(CCompressor &Compressor, CByteType *InData, CSizeType InDatLen);
void EncodeFileImpl(CCompressor &Compressor, const char *OriginFN, const char *EncodedFN);
void DecodeFileImpl(CCompressor &Compressor, const char *EncodedFN, const char *DecodedFN);