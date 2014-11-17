#pragma once

#include "iperipheral.h"

#include <vector>

namespace moss
{
    class TestPeripheral : public IPeripheral
    {
        public:
            virtual uint32_t send_command(uint32_t command);
            virtual void assign_memory(Memory *memory, uint32_t offset, uint32_t size); 

            enum Command
            {
                STATUS  = 0x10,
                READ    = 0x11,
                WRITE   = 0x12
            };

        protected:

            uint32_t read_disk();
            uint32_t write_disk();

            std::vector<uint32_t> _disk;

    };

}
