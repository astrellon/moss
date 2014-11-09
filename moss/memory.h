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

            void to_stream(std::ostream &os, uint32_t start = 0, uint32_t end = -1) const;

        private:
            uint32_t _size;
            std::vector<DataWord> _data;

        public:

            inline uint32_t size() const
            {
                return _size;
            }

            // INT
            inline int32_t int_data(uint32_t index) const
            {
                return _data.at(index).i;
            }
            inline void int_data(uint32_t index, int32_t value)
            {
                _data[index].i = value;
            }
            // UINT
            inline uint32_t uint_data(uint32_t index) const
            {
                return _data.at(index).u;
            }
            inline void uint_data(uint32_t index, uint32_t value)
            {
                _data[index].u = value;
            }
            // FLOAT
            inline float float_data(uint32_t index) const
            {
                return _data.at(index).f;
            }
            inline void float_data(uint32_t index, float value)
            {
                _data[index].f = value;
            }
    };
}
