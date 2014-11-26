#pragma once

#include <stdint.h>
#include <iostream>
#include <vector>

#include <moss/utils/common.h>

namespace moss
{
    class Memory
    {
        public:
            Memory(uint32_t size);
            ~Memory();

            /**
             * Zeroes out the memeory.
             */
            void zero();

            void to_stream(std::ostream &os, uint32_t start = 0, uint32_t end = -1) const;

        private:
            uint32_t _size;
            std::vector<DataWord> _data;

        public:

            FORCEINLINE uint32_t size() const
            {
                return _size;
            }

            // INT
            FORCEINLINE int32_t int_data(uint32_t index) const
            {
                return _data.at(index).i;
            }
            FORCEINLINE void int_data(uint32_t index, int32_t value)
            {
                _data[index].i = value;
            }
            // UINT
            FORCEINLINE uint32_t uint_data(uint32_t index) const
            {
                return _data.at(index).u;
            }
            FORCEINLINE void uint_data(uint32_t index, uint32_t value)
            {
                _data[index].u = value;
            }
            // FLOAT
            FORCEINLINE float float_data(uint32_t index) const
            {
                return _data.at(index).f;
            }
            FORCEINLINE void float_data(uint32_t index, float value)
            {
                _data[index].f = value;
            }
    };
}
