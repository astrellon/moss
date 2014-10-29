#pragma once

#include <stdint.h>

namespace moss
{
    class Memory;

    class Mmu
    {
        public:
            Mmu(uint32_t page_size);

            Memory *memory() const;
            void memory(Memory *memory);

            uint32_t page_size() const;

            uint32_t data(uint32_t index) const;
            void data(uint32_t index, uint32_t value);
            
        private:
            uint32_t _page_size;
            Memory *_memory;

    };
}
