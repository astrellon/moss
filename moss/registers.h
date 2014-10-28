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

            uint32_t program_counter() const;
            uint32_t program_counter_inc();
            void program_counter(uint32_t value);

            uint32_t stack_pointer() const;
            uint32_t stack_pointer_push();
            uint32_t stack_pointer_pop();
            void stack_pointer(uint32_t value);

            uint32_t data_pointer() const;
            void data_pointer(uint32_t value);

            uint32_t code_pointer() const;
            void code_pointer(uint32_t value);

            void to_stream(std::ostream &os) const;

        private:
            uint32_t _flags;
            uint32_t _stack_pointer;
            uint32_t _program_counter;
            uint32_t _data_pointer;
            uint32_t _code_pointer;
            std::array<uint32_t, 8> _int_regs;
    };
}
