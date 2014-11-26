#pragma once

#include <stdint.h>
#include <iostream>

#include "memory.h"

namespace moss
{
    class Mmu
    {
        public:
            Mmu(uint32_t page_bit_size);

            Memory *memory() const;
            void memory(Memory *memory);

            uint32_t page_bit_size() const;
            uint32_t page_size() const;

            uint32_t table_pointer() const;
            void table_pointer(uint32_t value);

            void to_stream(std::ostream &os, bool inVirtual = true, uint32_t start = 0, uint32_t end = -1) const;

        private:
            uint32_t _page_bit_size;
            uint32_t _page_bit_mask;
            uint32_t _page_size;
            uint32_t _table_pointer;
            bool _enable_mmu;
            Memory *_memory;

            FORCEINLINE uint32_t translate_index(uint32_t index) const
            {
                uint32_t page = index >> _page_bit_size;
                uint32_t index_offset = index & _page_bit_mask;;
                uint32_t translated = _memory->uint_data(_table_pointer + page);
                return (translated << _page_bit_size) + index_offset;
            }

        public:
            FORCEINLINE void enable_mmu(bool value)
            {
                _enable_mmu = value;
            }
            FORCEINLINE bool enable_mmu() const
            {
                return _enable_mmu;
            }

            FORCEINLINE int32_t int_data(uint32_t index) const
            {
                if (_enable_mmu)
                {
                    return _memory->int_data(translate_index(index));
                }
                return _memory->int_data(index);
            }
            FORCEINLINE void int_data(uint32_t index, int32_t value)
            {
                if (_enable_mmu)
                {
                    _memory->int_data(translate_index(index), value);
                }
                else
                {
                    _memory->int_data(index, value);
                }
            }
            FORCEINLINE uint32_t uint_data(uint32_t index) const
            {
                if (_enable_mmu)
                {
                    return _memory->uint_data(translate_index(index));
                }
                return _memory->uint_data(index);
            }
            FORCEINLINE void uint_data(uint32_t index, uint32_t value)
            {
                if (_enable_mmu)
                {
                    _memory->uint_data(translate_index(index), value);
                }
                else
                {
                    _memory->uint_data(index, value);
                }
            }
            FORCEINLINE float float_data(uint32_t index) const
            {
                if (_enable_mmu)
                {
                    return _memory->float_data(translate_index(index));
                }
                return _memory->float_data(index);
            }
            FORCEINLINE void float_data(uint32_t index, float value)
            {
                if (_enable_mmu)
                {
                    _memory->float_data(translate_index(index), value);
                }
                else
                {
                    _memory->float_data(index, value);
                }
            }
    };
}
