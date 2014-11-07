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

            /*
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
               void change_program_counter(int32_t value);
               */

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

        public:

            inline uint32_t num_word_reg() const
            {
                return static_cast<uint32_t>(_word_regs.size());
            }
            // INT 
            inline int32_t int_reg(uint32_t index) const
            {
                return _word_regs.at(index).i;
            }
            inline void int_reg(uint32_t index, int32_t value)
            {
                _word_regs[index].i = value;
            }
            // UINT
            inline uint32_t uint_reg(uint32_t index) const
            {
                return _word_regs.at(index).u;
            }
            inline void uint_reg(uint32_t index, uint32_t value)
            {
                _word_regs[index].u = value;
            }
            // FLOAT
            inline float float_reg(uint32_t index) const
            {
                return _word_regs.at(index).f;
            }
            inline void float_reg(uint32_t index, float value)
            {
                _word_regs[index].f = value;
            }

            inline uint32_t program_counter() const
            {
                return _program_counter;
            }
            inline uint32_t program_counter_inc()
            {
                return _program_counter++;
            }
            inline void program_counter(uint32_t value)
            {
                _program_counter = value;
            }
            inline void change_program_counter(int32_t value)
            {
                _program_counter += value;
            }
    };
}
