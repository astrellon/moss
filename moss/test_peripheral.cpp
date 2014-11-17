#include "test_peripheral.h"

#include <iostream>

namespace moss
{
    uint32_t TestPeripheral::send_command(uint32_t command)
    {
        switch (command)
        {
            default:
            case UNKNOWN:
                std::cout << "Unknown test peripheral command: " << command << "\n";
                return -1;
            case MEMORY_REQUEST:
                return 32;
            case STATUS:
                uint_data(0, 1);
                return 0;
            case READ:
                return read_disk();
            case WRITE:
                return write_disk();
        }
        return 0;
    }
    void TestPeripheral::assign_memory(Memory *memory, uint32_t offset, uint32_t size)
    {
        IPeripheral::assign_memory(memory, offset, size);

        _disk.reserve(32);
    }

    uint32_t TestPeripheral::read_disk()
    {
        for (auto i = 0u; i < _memory_size; i++)
        {
            uint_data(i, _disk[i]);
        }
        return 0;
    }
    uint32_t TestPeripheral::write_disk()
    {
        std::cout << "Writing to disk: ";
        for (auto i = 0u; i < _memory_size; i++)
        {
            _disk[i] = uint_data(i);
            std::cout << _disk[i] << ' ';
        }
        std::cout << "\n";
        return 0;
    }
}
