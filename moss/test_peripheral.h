#pragma once

#include "iperipheral.h"

namespace moss
{
    class TestPeripheral : public IPeripheral
    {
        public:
            virtual uint32_t send_command(uint32_t command);
    };

}
