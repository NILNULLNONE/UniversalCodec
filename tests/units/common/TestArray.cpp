#include "gtest/gtest.h"
#include "common/Array.h"

// TEST(SimpleTestCase, SimpleTest)
// {
//     EXPECT_EQ(2, 2);
// }

TEST(CASE_ArrayAdd, AddOne)
{
    CArray<int> Array;
    Array.Add(-1);
    EXPECT_EQ(Array.At(0), -1);
}

TEST(CASE_ArrayResize, Nest)
{
    CArray<CArray<CByteType>>NestArr = {};
    NestArr.Resize(2);
}

TEST(CASE_ArrayGet, Getter)
{
    CArray<int> Array;
    Array.AddElements({1,234,5,0,-27,-3});
    EXPECT_EQ(Array.At(0), 1);
    EXPECT_EQ(Array[1], 234);
    EXPECT_EQ(Array.At(5), -3);
    EXPECT_EQ(Array[5], -3);
}

TEST(CASE_ArrayRemove, RemoveAt)
{
    CArray<int> Array;
    Array.Add(3);
    Array.Add(5);
    Array.Add(8);
    EXPECT_EQ(Array.Count(), 3);
    Array.RemoveAt(1);
    EXPECT_EQ(Array.Count(), 2);
    EXPECT_EQ(Array.At(1), 8);
}

TEST(CASE_ArrayRemove, RemoveElement)
{
    CArray<int> Array;
    Array.AddElements({3, 9, 7, 4, 5});
    EXPECT_EQ(Array.Count() , 5);
    Array.RemoveElement(5);
    EXPECT_EQ(Array.Count(), 4);
}

TEST(CASE_ArrayCount, EmptyTest)
{
    CArray<int> EmptyArray;
    EXPECT_EQ(EmptyArray.Count(), 0);
}

TEST(CASE_ArrayCount, AddTest)
{
    CArray<int> Array;
    for(int i = 0; i < 5; ++i)
    {
        Array.Add(i);
    }
    EXPECT_EQ(Array.Count(), 5);
}
