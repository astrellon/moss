#pragma once

#include <stdint.h>
#include <vector>

#include "registers.h"
#include "mmu.h"

namespace moss
{
    class Memory;

    class Cpu
    {
        public:
            Cpu(uint32_t page_bit_size);
            ~Cpu();

            Registers &registers();
            const Registers &registers() const;

            Mmu &mmu();
            const Mmu &mmu() const;

            Memory *memory() const;
            void memory(Memory *memory);

            void run();
            void stop();
            bool is_running() const;

            void to_stream(std::ostream &os) const;

        private:
            bool _running;
            Registers _regs;
            Mmu _mmu;
            Memory *_memory;

            uint32_t next_pc_uint();
            int32_t next_pc_int();
            float next_pc_float();

            void do_run();

            void push_stack(uint32_t value);
            void push_stack_float(float value);
            uint32_t pop_stack();
            float pop_stack_float();
    };
}
