#pragma once

#include <stdint.h>
#include <array>
#include <vector>

#include "registers.h"
#include "mmu.h"

namespace moss
{
    class Memory;
    class IPeripheral;

    class CpuArm
    {
        public:
            CpuArm(uint32_t page_bit_size);
            ~CpuArm();

            Registers &registers();
            const Registers &registers() const;

            bool enable_mmu() const;
            void enable_mmu(bool enable);

            Mmu &mmu();
            const Mmu &mmu() const;

            Memory *memory() const;
            void memory(Memory *memory);

            void run();
            void stop();
            bool is_running() const;

            void to_stream(std::ostream &os) const;

            void install_peripheral(IPeripheral *perf);

            uint32_t named_reg(uint32_t name) const;
            void named_reg(uint32_t name, uint32_t value);

        private:
            bool _running;
            bool _enable_mmu;
            
            Registers _regs;
            Mmu _mmu;
            std::array<IPeripheral *, 16> _peripherals;
            
            Memory *_memory;

            void io_send(uint32_t reg_index, uint32_t perf_index, uint32_t command);

            FORCEINLINE uint32_t next_pc_uint()
            {
                return _mmu.uint_data(_regs.program_counter_inc());
            }
            FORCEINLINE int32_t next_pc_int()
            {
                return _mmu.int_data(_regs.program_counter_inc());
            }
            FORCEINLINE float next_pc_float()
            {
                return _mmu.float_data(_regs.program_counter_inc());
            }

            void do_run();

            void push_stack(uint32_t value);
            void push_stack_float(float value);
            uint32_t pop_stack();
            float pop_stack_float();
            
            void push_code_stack(uint32_t value);
            uint32_t pop_code_stack();

            static uint32_t s_int_bit_shift;
    };
}
