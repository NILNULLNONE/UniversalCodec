#include "gtest/gtest.h"
#include "codec/media/MediaCodecInterface.h"

TEST(CASE_PNGCodec, Debug)
{
    const char *PNGFilename = "C:\\Users\\35974\\Pictures\\tmp\\touxian.png";
    CPNGCodec Codec;
    Codec.DecodeFile(PNGFilename);
}