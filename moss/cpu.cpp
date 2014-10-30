#include "cpu.h"

#include <iostream>

#include "memory.h"

namespace moss
{
    Cpu::Cpu(uint32_t page_bit_size) :
        _running(false),
        _mmu(page_bit_size),
        _memory(nullptr)
    {
        _regs.zero();
    }
    Cpu::~Cpu()
    {
        stop();
    }

    Registers &Cpu::registers()
    {
        return _regs;
    }
    const Registers &Cpu::registers() const
    {
        return _regs;
    }

    const Mmu &Cpu::mmu() const
    {
        return _mmu;
    }

    Memory *Cpu::memory() const
    {
        return _memory;
    }
    void Cpu::memory(Memory *memory)
    {
        _memory = memory;
        _mmu.memory(memory);
    }

    void Cpu::run()
    {
        _running = true;
        do_run();
    }
    void Cpu::stop()
    {
        _running = false;
    }
    bool Cpu::is_running() const
    {
        return _running;
    }

    void Cpu::to_stream(std::ostream &os) const
    {
        os << "Cpu running: " << is_running() << "\n";
        _regs.to_stream(os);
    }

    uint32_t Cpu::next_int()
    {
        return _mmu.data(_regs.program_counter_inc());
    }

    void Cpu::push_stack(uint32_t value)
    {
        _mmu.data(_regs.stack_pointer_push(), value);
    }
    uint32_t Cpu::pop_stack()
    {
        return _mmu.data(_regs.stack_pointer_pop());
    }

    void Cpu::do_run()
    {
        while (_running)
        {
            uint32_t opcode = next_int();
            uint32_t ar1 = 0u;
            uint32_t ar2 = 0u;
            uint32_t ar3 = 0u;
            switch (opcode)
            {
                // Error or Halt {{{
                    default:
                    std::cout << "Unknown OPCODE: " << opcode << ", halting." << std::endl;
                case HALT:
                    stop();
                    break;
                // }}}

                // MOV Commands {{{
                    case MOV_R_R:
                    ar1 = next_int();
                    ar2 = next_int();
                    _regs.int_reg(ar2, _regs.int_reg(ar1));
                    break;
                case MOV_I_R:
                    ar1 = next_int();
                    ar2 = next_int();
                    _regs.int_reg(ar2, ar1);
                    break;
                case MOV_M_R:
                    ar1 = next_int();
                    ar2 = next_int();
                    _regs.int_reg(ar2, _mmu.data(_regs.int_reg(ar1)));
                    break;
                case MOV_R_M:
                    ar1 = next_int();
                    ar2 = next_int();
                    _mmu.data(_regs.int_reg(ar2), _regs.int_reg(ar1));
                    break;
                case MOV_I_M:
                    ar1 = next_int();
                    ar2 = next_int();
                    _mmu.data(ar2, ar1);
                    break;
                case MOV_M_M:
                    ar1 = next_int();
                    ar2 = next_int();
                    _mmu.data(ar2, _mmu.data(ar1));
                    break;
                // }}}

                // ADD Commands {{{
                case ADD_R_R:
                    ar1 = next_int();
                    ar2 = next_int();
                    _regs.int_reg(ar2, _regs.int_reg(ar1) + _regs.int_reg(ar2));
                    break;
                case ADD_R_R_R:
                    ar1 = next_int();
                    ar2 = next_int();
                    ar3 = next_int();
                    _regs.int_reg(ar3, _regs.int_reg(ar1) + _regs.int_reg(ar2));
                    break;
                // }}}

                // Stack commands {{{
                case PUSH_R:
                    ar1 = next_int();
                    push_stack(_regs.int_reg(ar1));
                    break;
                case PUSH_I:
                    ar1 = next_int();
                    push_stack(ar1);
                    break;
                case POP_R:
                    ar1 = next_int();
                    _regs.int_reg(ar1, pop_stack());
                    break;
                // }}}

                // JMP Commands {{{
                case JMP_R:
                    ar1 = next_int();
                    _regs.program_counter(_regs.int_reg(ar1));
                    break;
                case JMP_I:
                    ar1 = next_int();
                    _regs.program_counter(ar1);
                    break;
                // }}}

                // Debug commands {{{
                case PRINT_R:
                    ar1 = next_int();
                    std::cout << "Reg " << ar1 << ": " << _regs.int_reg(ar1);
                    break;
                // }}}

            }
        }
    }
}
