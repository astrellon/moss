#pragma once

#include <stdint.h>

namespace moss
{
    class Memory;

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

            uint32_t data(uint32_t index) const;
            void data(uint32_t index, uint32_t value);
            
        private:
            uint32_t _page_bit_size;
            uint32_t _page_bit_mask;
            uint32_t _page_size;
            uint32_t _table_pointer;
            Memory *_memory;

            uint32_t translate_index(uint32_t index) const;

    };
}
