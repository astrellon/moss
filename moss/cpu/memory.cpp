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

    void Memory::from_stream(std::istream &ss, uint32_t offset)
    {
        uint32_t i = 0u;
        while(ss)
        {
            uint32_t value;
            ss.read(reinterpret_cast<char *>(&value), sizeof(value));
            uint_data(offset + i++, value);
        }
    }
}
