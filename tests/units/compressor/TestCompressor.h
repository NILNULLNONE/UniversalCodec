#pragma once
#include "compressor/CompressorInterface.h"

void EncodeFileImpl(CCompressor &Compressor, const char *OriginFN, const char *EncodedFN);
void DecodeFileImpl(CCompressor &Compressor, const char *EncodedFN, const char *DecodedFN);