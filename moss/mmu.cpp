#include "mmu.h"

#include "memory.h"

namespace moss
{
    Mmu::Mmu(uint32_t page_bit_size) :
        _page_bit_size(page_bit_size),
        _table_pointer(0),
        _memory(nullptr)
    {
        _page_size = 1 << page_bit_size;
        _page_bit_mask = _page_size - 1;
    }

    uint32_t Mmu::page_bit_size() const
    {
        return _page_bit_size;
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

    uint32_t Mmu::data(uint32_t index) const
    {
        return _memory->data(translate_index(index));
    }
    void Mmu::data(uint32_t index, uint32_t value)
    {
        _memory->data(translate_index(index), value);
    }

    uint32_t Mmu::translate_index(uint32_t index) const
    {
        uint32_t page = index >> _page_bit_size;
        uint32_t index_offset = index & _page_bit_mask;;
        uint32_t translated = _memory->data(_table_pointer + page);
        return (translated << _page_bit_size) + index_offset;
    }
}
