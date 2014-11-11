#pragma once

#include <stdint.h>

namespace moss
{
    class IPeripheral
    {
        public:
            virtual uint32_t send_command(uint32_t command) = 0;
    };
}
