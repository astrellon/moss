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

            enum Opcodes
            {
                HALT,
            
                MOV_R_R,
                MOV_R_I,
                MOV_M_R,
                MOV_R_M,
                MOV_M_I,
                MOV_M_M,
                
                ADD_R_R,
                ADD_R_R_R,
                ADD_R_I,
                
                SUB_R_R,
                SUB_R_R_R,
                SUB_R_I,
                
                PUSH_R,
                PUSH_I,
                POP_R,

                CMP_R_R,
                CMP_R_I,
                CMP_I_R,

                JMP_R,
                JMP_I,
                JNE_R,
                JNE_I,
                JEQ_R,
                JEQ_I,
                JLT_R,
                JLT_I,
                JLE_R,
                JLE_I,
                JGT_R,
                JGT_I,
                JGE_R,
                JGE_I,
                
                PRINT_R
            };

            void to_stream(std::ostream &os) const;

        private:
            bool _running;
            Registers _regs;
            Mmu _mmu;
            Memory *_memory;

            uint32_t next_int();

            void do_run();

            void push_stack(uint32_t value);
            uint32_t pop_stack();
    };
}
