#include "memory.h"

#include <iomanip>
#include <stdexcept>

namespace moss
{
    Memory::Memory(uint32_t size) :
        _size(size)
    {
        _data = std::vector<DataWord>(size);
    }
    Memory::~Memory()
    {
    }

    void Memory::zero()
    {
        for (uint32_t i = 0u; i < _size; i++)
        {
            _data[i].u = 0u;
        }
    }

    uint32_t Memory::size() const
    {
        return _size;
    }

    // INT
    int32_t Memory::int_data(uint32_t index) const
    {
        return _data.at(index).i;
    }
    void Memory::int_data(uint32_t index, int32_t value)
    {
        if (index >= size())
        {
            throw std::out_of_range("Out of memory range");
        }
        _data[index].i = value;
    }
    // UINT
    uint32_t Memory::uint_data(uint32_t index) const
    {
        return _data.at(index).u;
    }
    void Memory::uint_data(uint32_t index, uint32_t value)
    {
        if (index >= size())
        {
            throw std::out_of_range("Out of memory range");
        }
        _data[index].u = value;
    }
    // FLOAT
    float Memory::float_data(uint32_t index) const
    {
        return _data.at(index).f;
    }
    void Memory::float_data(uint32_t index, float value)
    {
        if (index >= size())
        {
            throw std::out_of_range("Out of memory range");
        }
        _data[index].f = value;
    }

    void Memory::to_stream(std::ostream &os, uint32_t start, uint32_t end) const
    {
        if (end > size())
        {
            end = size();
        }

        os << "Memory: [" << start << ", " << end << "]\n";
        auto j = 0;
        for (auto i = start; i < end; i++)
        {
            os << std::setw(14) << _data[i].u << ' ';
            j++;
            if (j >= 8)
            {
                j = 0;
                os << std::endl;
            }
        }
    }
}
