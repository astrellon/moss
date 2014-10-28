#pragma once

#include <stdint.h>
#include <vector>

#include "registers.h"

namespace moss
{
    class Memory;

    class Cpu
    {
        public:
            Cpu();
            ~Cpu();

            Memory *memory() const;
            void memory(Memory *memory);

            void run();
            void stop();
            bool is_running() const;

            enum Opcodes
            {
                HALT,
                MOV_R_R,
                MOV_I_R,
                MOV_M_R,
                MOV_R_M,
                MOV_I_M,
                MOV_M_M,
                ADD_R_R,
                ADD_R_R_R,
                PUSH_R,
                POP_R
            };

            void to_stream(std::ostream &os) const;

        private:
            bool _running;
            Registers _regs;
            Memory *_memory;

            uint32_t next_int();

            void do_run();

            void push_stack(uint32_t value);
            uint32_t pop_stack();
    };
}
