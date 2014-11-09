#pragma once

#include <stdint.h>
#include <vector>

#include "registers.h"
#include "mmu.h"

namespace moss
{
    class Memory;

    class CpuArm
    {
        public:
            CpuArm(uint32_t page_bit_size);
            ~CpuArm();

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

            inline uint32_t next_pc_uint()
            {
                return _mmu.uint_data(_regs.program_counter_inc());
            }
            inline int32_t next_pc_int()
            {
                return _mmu.int_data(_regs.program_counter_inc());
            }
            inline float next_pc_float()
            {
                return _mmu.float_data(_regs.program_counter_inc());
            }

            void do_run();

            void push_stack(uint32_t value);
            void push_stack_float(float value);
            uint32_t pop_stack();
            float pop_stack_float();

            static uint32_t s_int_bit_shift;
    };
}
