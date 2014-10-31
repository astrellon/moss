#pragma once

#include <stdint.h>
#include <iostream>
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

            uint32_t add_command(uint32_t index, uint32_t cmd);
            uint32_t add_command(uint32_t index, uint32_t cmd, uint32_t arg1);
            uint32_t add_command(uint32_t index, uint32_t cmd, uint32_t arg1, uint32_t arg2);
            uint32_t add_command(uint32_t index, uint32_t cmd, uint32_t arg1, uint32_t arg2, uint32_t arg3);

            void to_stream(std::ostream &os, uint32_t start = 0, uint32_t end = -1) const;

        private:
            uint32_t _size;
            std::vector<uint32_t> _data;
    };
}
