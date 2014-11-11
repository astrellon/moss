#include "test_peripheral.h"

#include <iostream>

namespace moss
{
    uint32_t TestPeripheral::send_command(uint32_t command)
    {
        std::cout << "TEST PERF: " << std::dec << command << "\n";
        return 0;
    }
}
