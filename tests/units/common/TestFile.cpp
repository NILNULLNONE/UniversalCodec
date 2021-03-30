#include "gtest/gtest.h"
#include "common/File.h"

TEST(CASE_File, FileReadWrite)
{
    const char *Filename = "TestFile.dat";
    CByteType TestBytes[] = {1, 3, 9, 19, 37, 88, 7};
    auto Count = sizeof(TestBytes) / sizeof(CByteType);
    CFile::SaveDataToFile(Filename, TestBytes, sizeof(TestBytes));

    CByteType* TestOutBytes = nullptr;
    CSizeType Size = 0;
    CFile::LoadDataFromFile(Filename, TestOutBytes, Size);
    EXPECT_EQ(Size, sizeof(TestBytes));
    EXPECT_EQ(TestOutBytes[0], 1);
    EXPECT_EQ(TestOutBytes[1], 3);
    EXPECT_EQ(TestOutBytes[2], 9);
    EXPECT_EQ(TestOutBytes[6], 7);
}