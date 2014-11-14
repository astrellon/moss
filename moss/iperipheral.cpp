#include "iperipheral.h"

namespace moss
{
    void IPeripheral::assign_memory(Memory *memory, uint32_t offset, uint32_t size)
    {
        _memory = memory;
        _memory_offset = offset;
        _memory_size = size;
    }
}

