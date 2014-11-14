#pragma once

#include "iperipheral.h"

namespace moss
{
    class TestPeripheral : public IPeripheral
    {
        public:
            virtual uint32_t send_command(uint32_t command);

            enum Command
            {
                UNKNOWN,
                STATUS,
                READ,
                WRITE
            };

        protected:

            uint32_t read_disk();
            uint32_t write_disk();

    };

}
