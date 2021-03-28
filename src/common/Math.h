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
}