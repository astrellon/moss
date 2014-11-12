#include "mmu.h"

#include <iomanip>

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
    
    void Mmu::to_stream(std::ostream &os, bool in_virtual, uint32_t start, uint32_t end) const
    {
        if (end > _memory->size())
        {
            end = _memory->size();
        }

        os << std::dec << "Mmu: [" << start << ", " << end << "]\n";
        auto j = 0;
        auto p = 0;
        auto page = 0;
        for (auto i = start; i < end; i++)
        {
            if (p == 0)
            {
                os << "Page: " << page << ", " << _memory->int_data(_table_pointer + page) << "\n";
            }
            p++;
            if (p >= _page_size)
            {
                p = 0;
                page++;
            }

            if (in_virtual)
            {
                os << std::setw(10) << int_data(i) << ' ';
            }
            else
            {
                os << std::setw(10) << _memory->int_data(i) << ' ';
            }
            j++;
            if (j >= 8)
            {
                j = 0;
                os << std::endl;
            }
        }
    }
}
