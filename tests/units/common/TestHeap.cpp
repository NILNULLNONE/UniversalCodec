#include "gtest/gtest.h"
#include "common/Heap.h"
#include <cstdio>

TEST(CASE_HeapConstruct, Construct)
{
    CArray<int> Array;
    Array.AddElements({7, 1, 3, 8, 9, 11, 0, -1, 234, 2, 1, 93});
    CBinaryHeap<int> Heap(Array.GetData(), Array.Count());
    CArray<int> HeapArray = Heap.ToArray();
    EXPECT_EQ(HeapArray.Count(), 12);
    printf("%d, %d, %d, %d, "
           "%d, %d, %d, %d, "
           "%d, %d, %d, %d\n",
           HeapArray[0],
           HeapArray[1],
           HeapArray[2],
           HeapArray[3],
           HeapArray[4],
           HeapArray[5],
           HeapArray[6],
           HeapArray[7],
           HeapArray[8],
           HeapArray[9],
           HeapArray[10],
           HeapArray[11]
        );
}

TEST(CASE_HeapPop, Sort)
{
    CArray<int> Array;
    Array.AddElements({7, 1, 3, 8, 9, 11, 0, -1, 234, 2, 1, 93});
    CBinaryHeap<int> Heap(Array.GetData(), Array.Count());
    CArray<int> HeapArray;
    HeapArray.Resize(Array.Count());
    for(int i = 0; i < HeapArray.Count(); ++i)
    {
        HeapArray[i] = Heap.Pop();
    }
    printf("%d, %d, %d, %d, "
           "%d, %d, %d, %d, "
           "%d, %d, %d, %d\n",
           HeapArray[0],
           HeapArray[1],
           HeapArray[2],
           HeapArray[3],
           HeapArray[4],
           HeapArray[5],
           HeapArray[6],
           HeapArray[7],
           HeapArray[8],
           HeapArray[9],
           HeapArray[10],
           HeapArray[11]);
}