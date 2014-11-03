#pragma once

#include <stdint.h>
#include <array>
#include <iostream>
#include "common.h"

namespace moss
{
    class Registers
    {
        public:
            Registers();

            void zero();

            uint32_t flags() const;

            bool zero_flag() const;
            void zero_flag(bool flag);
            bool neg_flag() const;
            void neg_flag(bool flag);

            uint32_t num_word_reg() const;
            int32_t int_reg(uint32_t index) const;
            void int_reg(uint32_t index, int32_t value);
            uint32_t uint_reg(uint32_t index) const;
            void uint_reg(uint32_t index, uint32_t value);
            float float_reg(uint32_t index) const;
            void float_reg(uint32_t index, float value);

            uint32_t program_counter() const;
            uint32_t program_counter_inc();
            void program_counter(uint32_t value);

            uint32_t stack_pointer() const;
            uint32_t stack_pointer_push();
            uint32_t stack_pointer_pop();
            void stack_pointer(uint32_t value);

            void to_stream(std::ostream &os) const;

            enum FlagBits
            {
                ZERO_FLAG = 0x01,
                NEG_FLAG = 0x02
            };

        private:
            uint32_t _flags;
            uint32_t _stack_pointer;
            uint32_t _program_counter;
            std::array<DataWord, 16> _word_regs;
    };
}
