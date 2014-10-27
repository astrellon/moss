#pragma once

#include <stdint.h>
#include <vector>

namespace moss
{
    class Memory
    {
        public:
            Memory(uint32_t size);
            ~Memory();

            void zero();

            uint32_t size() const;

            uint32_t data(uint32_t index) const;
            void data(uint32_t index, uint32_t value);

        private:
            uint32_t _size;
            std::vector<uint32_t> _data;
    };
}
