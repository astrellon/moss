#include "memory.h"

namespace moss
{
    Memory::Memory(uint32_t size) :
        _size(size)
    {
        _data = std::vector<uint32_t>(size);
    }
    Memory::~Memory()
    {
    }

    void Memory::zero()
    {
        for (uint32_t i = 0u; i < _size; i++)
        {
            _data[i] = 0u;
        }
    }

    uint32_t Memory::size() const
    {
        return _size;
    }

    uint32_t Memory::data(uint32_t index) const
    {
        return _data[index];
    }
    void Memory::data(uint32_t index, uint32_t value)
    {
        _data[index] = value;
    }
}
