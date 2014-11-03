#pragma once

#include <stdint.h>
#include <iostream>
#include <vector>

#include "common.h"

namespace moss
{
    class Memory
    {
        public:
            Memory(uint32_t size);
            ~Memory();

            void zero();

            uint32_t size() const;

            int32_t int_data(uint32_t index) const;
            void int_data(uint32_t index, int32_t value);
            uint32_t uint_data(uint32_t index) const;
            void uint_data(uint32_t index, uint32_t value);
            float float_data(uint32_t index) const;
            void float_data(uint32_t index, float value);

            void to_stream(std::ostream &os, uint32_t start = 0, uint32_t end = -1) const;

        private:
            uint32_t _size;
            std::vector<DataWord> _data;
    };
}
