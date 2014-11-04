#pragma once

#include <iostream>
#include <stdint.h>

namespace moss
{
    class Mmu;

    class Disassembler
    {
        public:

            static void to_stream(std::ostream &ss, Mmu *mmu, uint32_t start, uint32_t end);
    };
}
