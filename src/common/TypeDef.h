#pragma once
#include <cstdint>

using CSizeType = uint32_t;
using CSizeType64 = uint64_t;
using CBitType = int8_t;
using CByteType = int8_t;

template<typename Type0, typename Type1>
struct CPair
{
    Type0 Left;
    Type1 Right;
};