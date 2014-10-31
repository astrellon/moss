#include "memory.h"

#include <iomanip>

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

    uint32_t Memory::add_command(uint32_t index, uint32_t cmd)
    {
        _data[index] = cmd;
        return index + 1;
    }
    uint32_t Memory::add_command(uint32_t index, uint32_t cmd, uint32_t arg1)
    {
        _data[index] = cmd;
        _data[index + 1] = arg1;
        return index + 2;
    }
    uint32_t Memory::add_command(uint32_t index, uint32_t cmd, uint32_t arg1, uint32_t arg2)
    {
        _data[index] = cmd;
        _data[index + 1] = arg1;
        _data[index + 2] = arg2;
        return index + 3;
    }
    uint32_t Memory::add_command(uint32_t index, uint32_t cmd, uint32_t arg1, uint32_t arg2, uint32_t arg3)
    {
        _data[index] = cmd;
        _data[index + 1] = arg1;
        _data[index + 2] = arg2;
        _data[index + 3] = arg3;
        return index + 4;
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
            os << std::setw(4) << _data[i] << ' ';
            j++;
            if (j >= 8)
            {
                j = 0;
                os << std::endl;
            }
        }
    }
}
