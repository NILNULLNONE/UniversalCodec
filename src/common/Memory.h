#pragma once
#include <cstdlib>
#include <cstring>
#include "TypeDef.h"

namespace CMemory
{
    template<typename ElementType>
    ElementType* Malloc(int ElementCount)
    {
        return static_cast<ElementType *>(malloc(sizeof(ElementType) * ElementCount));
    }

    template<typename ElementType>
    ElementType* Realloc(ElementType* InDataPtr, int ElementCount)
    {
        return static_cast<ElementType*>(realloc(InDataPtr, sizeof(ElementType) * ElementCount));
    }

    template <typename ElementType>
    void SafeFree(ElementType *&InPtr)
    {
        if (!InPtr)
            return;
        free(InPtr);
        InPtr = nullptr;
    }

    CSizeType Copy(void *Dst, const void *Src, CSizeType Size);

    void Zero(void* Dst, CSizeType InLen);

    template<typename ElementType>
    void Fill(ElementType* DstData, CSizeType DstLen, const ElementType& InValue)
    {
        for(int i = 0; i < DstLen; ++i)
        {
            DstData[i] = InValue;
        }
    }

    template<typename ElementType>
    void Swap(ElementType& Left, ElementType& Right)
    {
        ElementType Temp = Left;
        Left = Right;
        Right = Temp;
    }

    template<typename Type>
    CSizeType Write(void* DstData, const Type& Value)
    {
        CSizeType Size = sizeof(Value);
        memcpy(DstData, &Value, Size);
        return Size;
    }

    template<typename Type>
    CSizeType Read(const void* SrcData, Type& Value)
    {
        CSizeType Size = sizeof(Value);
        memcpy(&Value, SrcData, Size);
        return Size;
    }
};