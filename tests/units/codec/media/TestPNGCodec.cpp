#include "gtest/gtest.h"
#include "codec/media/MediaCodecInterface.h"
#include "TestData.h"

TEST(CASE_PNGCodec, Debug)
{
    CPNGCodec Codec;
    Codec.DecodeFile(GTestData.PNGFN0.CStr());
}