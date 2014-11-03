#pragma once

#define TO_FLOAT(x) reinterpret_cast<float&>(x)
#define TO_UINT(x) reinterpret_cast<uint32_t&>(x)
#define TO_INT(x) reinterpret_cast<int32_t&>(x)

#include <stdint.h>

namespace moss
{
    typedef union 
    {
        uint32_t u;
        int32_t i;
        float f;
    } DataWord;
}
