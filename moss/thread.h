#pragma once

#include <iostream>
#include <stdint.h>

#include "registers.h"

namespace moss
{
    class Thread
    {
        public:
            Thread();
            ~Thread();

            uint32_t program_counter() const;
            uint32_t program_counter_inc();
            void program_counter(uint32_t counter);

            uint32_t stack_pointer() const;
 
            Registers &registers();
            const Registers &registers() const;
            void registers(const Registers &regs);

            void to_stream(std::ostream &os) const;

        private:
            uint32_t _program_counter;
            uint32_t _stack_pointer;

            Registers _regs;
    };
}
