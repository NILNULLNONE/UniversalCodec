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
}