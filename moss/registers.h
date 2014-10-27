#pragma once

#include <stdint.h>
#include <array>
#include <iostream>

namespace moss
{
    class Registers
    {
        public:
            Registers();

            void zero();

            uint32_t flags() const;

            uint32_t num_int_reg() const;
            uint32_t int_reg(uint32_t index) const;
            void int_reg(uint32_t index, uint32_t value);

            void to_stream(std::ostream &os) const;

        private:
            uint32_t _flags;
            std::array<uint32_t, 8> _int_regs;
    };
}
