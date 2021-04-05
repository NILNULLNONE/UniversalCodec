#pragma once
#include "Memory.h"

namespace CMath
{
    template<typename Type>
    const Type& Min(const Type& Left, const Type& Right)
    {
        return Left < Right ? Left : Right;
    }

    template<typename Type>
    const Type& Max(const Type& Left, const Type& Right)
    {
        return Left > Right ? Left : Right;
    }

    template<typename Type>
    struct CLessComparator
    {
        bool operator()(const Type& Left, const Type& Right)const
        {
            return Left < Right;
        }
    };

    template <typename Type>
    struct CGreaterComparator
    {
        bool operator()(const Type& Left, const Type& Right)const
        {
            return Left > Right;
        }
    };

    template <typename Type>
    Type DivideAndRoundUp(Type Up, Type Down)
    {
        if (Up == 0)return 0;
        CException::Check(Up > 0 && Down != 0);
        return (Up - 1) / Down + 1;
    }

    template<typename Type>
    bool InRange(Type Value, Type Left, Type Right)
    {
        return Left <= Value && Value <= Right;
    }

    template<typename Type>
    Type Align(Type Value, Type Alignment)
    {
        return CMath::DivideAndRoundUp(Value, Alignment) * Alignment;
    }

    template <typename Type>
    CSizeType BitCount(Type Value)
    {
        CSizeType Count = 0;
        while (Value > 0)
        {
            Count++;
            Value >>= 1;
        }
        return Count;
    }

    template <typename Type>
    CSizeType CountZeroFromLeft(const Type Value)
    {
        CSizeType Bits = sizeof(Type) * 8;
        return Bits - BitCounts(Value);
    }
}