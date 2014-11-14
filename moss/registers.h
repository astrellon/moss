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

            uint32_t stack_pointer() const;
            uint32_t stack_pointer_push();
            uint32_t stack_pointer_pop();
            void stack_pointer(uint32_t value);

            void to_stream(std::ostream &os) const;
            
            enum Flags
            {
                FLAG_UNKNOWN    = -1,
                FLAG_ZERO       = 0x01,
                FLAG_NEGATIVE   = 0x02,
                FLAG_CARRY      = 0x04,
                FLAG_OVERFLOW   = 0x08,
                FLAG_ENABLE_MMU = 0x10
            };

        private:
            uint32_t _flags;
            uint32_t _stack_pointer;
            uint32_t _program_counter;
            std::array<DataWord, 16> _word_regs;

        public:

            FORCEINLINE uint32_t num_word_reg() const
            {
                return static_cast<uint32_t>(_word_regs.size());
            }
            // INT 
            FORCEINLINE int32_t int_reg(uint32_t index) const
            {
                return _word_regs.at(index).i;
            }
            FORCEINLINE void int_reg(uint32_t index, int32_t value)
            {
                _word_regs[index].i = value;
            }
            // UINT
            FORCEINLINE uint32_t uint_reg(uint32_t index) const
            {
                return _word_regs.at(index).u;
            }
            FORCEINLINE void uint_reg(uint32_t index, uint32_t value)
            {
                _word_regs[index].u = value;
            }
            // FLOAT
            FORCEINLINE float float_reg(uint32_t index) const
            {
                return _word_regs.at(index).f;
            }
            FORCEINLINE void float_reg(uint32_t index, float value)
            {
                _word_regs[index].f = value;
            }

            FORCEINLINE uint32_t program_counter() const
            {
                return _program_counter;
            }
            FORCEINLINE uint32_t program_counter_inc()
            {
                return _program_counter++;
            }
            FORCEINLINE void program_counter(uint32_t value)
            {
                _program_counter = value;
            }
            FORCEINLINE void change_program_counter(int32_t value)
            {
                _program_counter += value;
            }
            
            FORCEINLINE bool zero_flag() const
            {
                return (_flags & FLAG_ZERO) > 0;
            }
            FORCEINLINE void zero_flag(bool value)
            {
                _flags = value ? _flags | FLAG_ZERO : _flags & ~FLAG_ZERO;
            }
            FORCEINLINE bool neg_flag() const
            {
                return (_flags & FLAG_NEGATIVE) > 0;
            }
            FORCEINLINE void neg_flag(bool value)
            {
                _flags = value ? _flags | FLAG_NEGATIVE : _flags & ~FLAG_NEGATIVE;
            }
            FORCEINLINE bool enable_mmu() const
            {
                return (_flags & FLAG_ENABLE_MMU) > 0;
            }
            FORCEINLINE void enable_mmu(bool value)
            {
                _flags = value ? _flags | FLAG_ENABLE_MMU : _flags & ~FLAG_ENABLE_MMU;
            }

            FORCEINLINE bool flag(uint32_t mask)
            {
                return (_flags & mask) > 0;
            }
            FORCEINLINE void flag(uint32_t mask, bool value)
            {
                _flags = value ? _flags | mask : _flags & ~mask;
            }
    };
}
