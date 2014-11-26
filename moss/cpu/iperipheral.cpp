#include "iperipheral.h"

namespace moss
{
    IPeripheral::IPeripheral() :
        _memory(nullptr),
        _memory_offset(0),
        _memory_size(0)
    {

    }

    void IPeripheral::assign_memory(Memory *memory, uint32_t offset, uint32_t size)
    {
        _memory = memory;
        _memory_offset = offset;
        _memory_size = size;
        std::cout << "Assigned memory: " << offset << ", " << size << "\n";
    }

    bool IPeripheral::assigned() const
    {
        return _memory != nullptr;
    }
}

