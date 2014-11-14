#include "test_peripheral.h"

#include <iostream>

namespace moss
{
    uint32_t TestPeripheral::send_command(uint32_t command)
    {
        //std::cout << "TEST PERF: " << std::dec << command << "\n";
        //return command * 2;
        switch (command)
        {
            default:
            case UNKNOWN:
                std::cout << "Unknown test peripheral command: " << command << "\n";
                return -1;
            case STATUS:
                uint_data(0, 1);
                return 0;
            case READ:
                return read_memory();
            case WRITE:
                return write_memory();
        }
        return 0;
    }

    uint32_t TestPeripheral::read_disk()
    {

        return 0;
    }
    uint32_t TestPeripheral::write_disk()
    {
        return 0;
    }
}
