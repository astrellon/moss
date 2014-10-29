#include "mmu.h"

namespace moss
{
    Mmu::Mmu(uint32_t page_size) :
        _page_size(page_size),
        _memory(nullptr)
    {

    }

    uint32_t Mmu::page_size() const
    {
        return _page_size;
    }

    Memory *Mmu::memory() const
    {
        return _memory;
    }
    void Mmu::memory(Memory *memory)
    {
        _memory = memory;
    }
}
