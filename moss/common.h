#pragma once

#include <stdint.h>

namespace moss
{
    typedef union DataWord_u
    {
        uint32_t u;
        int32_t i;
        float f;

        DataWord_u() : u(0u) {}
        DataWord_u(float value) : f(value) {}
        DataWord_u(uint32_t value) : u(value) {}
        DataWord_u(int32_t value) : i(value) {}

    } DataWord;
}
