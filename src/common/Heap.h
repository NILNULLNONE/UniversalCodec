#pragma once
#include "Array.h"

template<typename ElementType, typename ComparatorType = CMath::CLessComparator<ElementType>>
class CBinaryHeap
{
public:
    struct COperation
    {
        static CSizeType ParentIndex(CSizeType InIndex)
        {
            return InIndex >> 1;
        }

        static CSizeType LeftChildIndex(CSizeType InIndex)
        {
            return InIndex << 1;
        }

        static CSizeType RightChildIndex(CSizeType InIndex)
        {
            return (InIndex << 1) | 1;
        }

        static CSizeType LastNonLeafIndex(CSizeType HeapArraySize)
        {
            return ParentIndex(HeapArraySize);
        }

        static void BuildHeap(ElementType *Elements, CSizeType HeapArraySize, const ComparatorType &InComparator)
        {
            for (int i = LastNonLeafIndex(HeapArraySize); i >= 1; --i)
            {
                ShiftDown(Elements, HeapArraySize, i, InComparator);
            }
        }

        static void ShiftDown(ElementType *Elements, CSizeType HeapArraySize, CSizeType InIndex, const ComparatorType &InComparator)
        {
            auto Left = LeftChildIndex(InIndex), Right = RightChildIndex(InIndex);
            if(Left > HeapArraySize)return;
            auto LeftElement = Elements[Left], Element = Elements[InIndex];
            if (Left <= HeapArraySize && Right <= HeapArraySize)
            {
                auto RightElement = Elements[Right];
                if (!InComparator(LeftElement, RightElement))
                {
                    CMemory::Swap(Left, Right);
                    CMemory::Swap(LeftElement, RightElement);
                }
            }
            if (!InComparator(Element, LeftElement))
            {
                CMemory::Swap(Elements[Left], Elements[InIndex]);
                ShiftDown(Elements, HeapArraySize, Left, InComparator);
            }
        }

        static void ShiftUp(ElementType *Elements, CSizeType HeapArraySize, CSizeType InIndex, const ComparatorType& InComparator)
        {
            auto ParentIdx = ParentIndex(InIndex);
            if(ParentIdx < 1)return;
            auto& Parent = Elements[ParentIdx], &Element = Elements[InIndex];
            if (!InComparator(Parent, Element))
            {
                CMemory::Swap(Parent, Element);
                ShiftUp(Elements, HeapArraySize, ParentIdx, InComparator);
            }
        }
    };
public:
    CBinaryHeap()
    {
        Data.Resize(1);
    }

    CBinaryHeap(const ElementType *InData, CSizeType ElementCount)
        : Data(InData, ElementCount, 1)
    {
        COperation::BuildHeap(GetArrayData(), GetHeapArraySize(), Comparator);
    }

    ~CBinaryHeap()
    {
    }

    void Insert(const ElementType& InElement)
    {
        Data.Add(InElement);
        COperation::ShiftUp(GetArrayData(), GetHeapArraySize(), GetHeapArraySize(), Comparator);
    }

    ElementType Pop()
    {
        if(IsEmpty())return ElementType();
        CMemory::Swap(Data.Second(), Data.Last());
        ElementType Ret = Data.Last();
        Data.RemoveAt(Data.Count() - 1);
        if(!IsEmpty())
            COperation::ShiftDown(GetArrayData(), GetHeapArraySize(), 1, Comparator);
        return Ret;
    }

    const ElementType& Top() const
    {
        return Data.Second();
    }

    bool IsEmpty() const
    {
        return Data.Count() <= 1;
    }

    CArray<ElementType> ToArray()
    {
        return CArray<ElementType>(GetHeapArrayData(), GetHeapArraySize());
    }

private:
    ElementType* GetArrayData()
    {
        return Data.GetData();
    }

    ElementType* GetHeapArrayData()
    {
        return Data.GetData() + 1;
    }

    CSizeType GetHeapArraySize()
    {
        return Data.Count() - 1;
    }

private:
    CArray<ElementType> Data = {};
    ComparatorType Comparator;
};