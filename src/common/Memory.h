#pragma once
#include <cstdlib>
#include <cstring>
#include "TypeDef.h"

namespace CMemory
{
    CSizeType Copy(void *Dst, const void *Src, CSizeType Size);

    void Zero(void *Dst, CSizeType InLen);

    int Compare(const void *Buf1, const void *Buf2, CSizeType Size);
    
    bool Equal(const void *Buf1, const void *Buf2, CSizeType Size);

    void SwapEndian(void *Data, CSizeType DataLen);

    template<typename ElementType>
    ElementType* Malloc(int ElementCount)
    {
        return static_cast<ElementType *>(malloc(sizeof(ElementType) * ElementCount));
    }

    template<typename ElementType>
    CSizeType CopyObjects(ElementType *Dst, const ElementType *Src, CSizeType Size)
    {
        for(int i = 0; i < Size; ++i)
        {
            Dst[i] = Src[i];
        }
    }

    template<typename ElementType>
    ElementType* MallocObjects(int ElementCount)
    {
        return new ElementType[ElementCount];
    }

    template<typename ElementType>
    ElementType* Realloc(ElementType* InDataPtr, int ElementCount)
    {
        return static_cast<ElementType*>(realloc(InDataPtr, sizeof(ElementType) * ElementCount));
    }

    template <typename ElementType>
    void SafeFree(ElementType *&InPtr)
    {
        if (!InPtr)return;
        free(InPtr);
        InPtr = nullptr;
    }

    template <typename ElementType>
    void SafeFreeObjects(ElementType *&InPtr)
    {
        if(!InPtr)return;
        delete[]InPtr;
        InPtr = nullptr;
    }

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

    template<typename Type>
    Type* Cast(void* Ptr)
    {
        return static_cast<Type*>(Ptr);
    }

    template <typename Type>
    const Type *Cast(const void *Ptr)
    {
        return static_cast<Type *>(Ptr);
    }

    template<typename Type>
    Type* ForceCast(void* Ptr)
    {
        return reinterpret_cast<Type*>(Ptr);
    }

    template <typename Type>
    const Type *ForceCast(const void *Ptr)
    {
        return reinterpret_cast<const Type *>(Ptr);
    }

    template <typename Type>
    void SwapEndian(Type &Value)
    {
        SwapEndian(&Value, sizeof(Type));
    }
};