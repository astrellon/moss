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

            bool remote_debugger() const;
            void remote_debugger(bool value);

            int32_t run();
            void stop();
            bool is_running() const;

            void to_stream(std::ostream &os) const;

            void install_peripheral(IPeripheral *perf);

            uint32_t named_reg(uint32_t name) const;
            void named_reg(uint32_t name, uint32_t value);

            enum DebugState
            {
                DEBUG_NONE,
                DEBUG_CONTINUE,
                DEBUG_STEP
            };

            void debug_state(DebugState state);
            DebugState debug_state() const;

            void stack_size(uint32_t size);
            uint32_t stack_size() const;

            void code_stack_size(uint32_t size);
            uint32_t code_stack_size() const;

        private:
            bool _running;
            bool _enable_mmu;
            
            Registers _regs;
            Mmu _mmu;
            
            std::array<IPeripheral *, 16> _peripherals;
            std::array<uint32_t, 16> _interrupts;
            uint32_t _interrupt_return;
            
            bool _remote_debugger;
            DebugState _debug_state;

            uint32_t _stack_size;
            uint32_t _code_stack_size;
            Memory *_memory;

            void io_send(uint32_t reg_index, uint32_t perf_index, int32_t command);

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

            int32_t do_run();

            void push_stack(int32_t value);
            void push_stack_float(float value);
            int32_t pop_stack();
            float pop_stack_float();
            
            void push_code_stack(uint32_t value);
            uint32_t pop_code_stack();

            static uint32_t s_int_bit_shift;

            void update_stack_pointers();
    };
}
