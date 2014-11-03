#include "cpu.h"

#include <iostream>

#include "memory.h"
#include "common.h"

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

    Mmu &Cpu::mmu()
    {
        return _mmu;
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

    uint32_t Cpu::next_pc()
    {
        return _mmu.uint_data(_regs.program_counter_inc());
    }

    void Cpu::push_stack(uint32_t value)
    {
        _mmu.uint_data(_regs.stack_pointer_push(), value);
    }
    uint32_t Cpu::pop_stack()
    {
        return _mmu.uint_data(_regs.stack_pointer_pop());
    }

    void Cpu::do_run()
    {
        uint32_t arg1 = 0u;
        uint32_t arg2 = 0u;
        uint32_t arg3 = 0u;
        while (_running)
        {
            uint32_t opcode = next_pc();
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
                    // reg[arg1] = reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg2));
                    break;
                case MOV_R_I:
                    // reg[arg1] = arg2
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.uint_reg(arg1, arg2);
                    break;
                case MOV_M_R:
                    // mem[reg[arg1]] = reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _mmu.uint_data(_regs.uint_reg(arg1), _regs.uint_reg(arg2));
                    break;
                case MOV_R_M:
                    // reg[arg1] = mem[reg[arg2]]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.uint_reg(arg1, _mmu.uint_data(_regs.uint_reg(arg2)));
                    break;
                case MOV_M_I:
                    // mem[reg[arg1]] = arg2
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _mmu.uint_data(_regs.uint_reg(arg1), arg2);
                    break;
                case MOV_M_M:
                    // mem[reg[arg1]] = mem[reg[arg2]]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _mmu.uint_data(_regs.uint_reg(arg1), _mmu.uint_data(_regs.uint_reg(arg2)));
                    break;
                // }}}
                
                // MOVF Commands {{{
                case MOVF_R_R:
                    // reg[arg1] = reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg2));
                    break;
                case MOVF_R_I:
                    // reg[arg1] = arg2
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.float_reg(arg1, TO_FLOAT(arg2));
                    break;
                case MOVF_M_R:
                    // mem[reg[arg1]] = reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _mmu.float_data(_regs.float_reg(arg1), _regs.float_reg(arg2));
                    break;
                case MOVF_R_M:
                    // reg[arg1] = mem[reg[arg2]]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.float_reg(arg1, _mmu.float_data(_regs.float_reg(arg2)));
                    break;
                case MOVF_M_I:
                    // mem[reg[arg1]] = arg2
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _mmu.float_data(_regs.float_reg(arg1), arg2);
                    break;
                case MOVF_M_M:
                    // mem[reg[arg1]] = mem[reg[arg2]]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _mmu.float_data(_regs.float_reg(arg1), _mmu.float_data(_regs.float_reg(arg2)));
                    break;
                // }}}

                // Unit Converstions {{{
                case UINT_FLOAT_R:
                    arg1 = next_pc();
                    {
                        uint32_t value = _regs.uint_reg(arg1);
                        _regs.float_reg(arg1, static_cast<float>(value));
                    }
                    break;
                case UINT_FLOAT_R_R:
                    arg1 = next_pc();
                    arg2 = next_pc();
                    {
                        uint32_t value = _regs.uint_reg(arg2);
                        _regs.float_reg(arg1, static_cast<float>(value));
                    }
                    break;
                case FLOAT_UINT_R:
                    arg1 = next_pc();
                    {
                        float value = _regs.float_reg(arg1);
                        _regs.uint_reg(arg1, static_cast<uint32_t>(value));
                    }
                    break;
                case FLOAT_UINT_R_R:
                    arg1 = next_pc();
                    arg2 = next_pc();
                    {
                        float value = _regs.float_reg(arg2);
                        _regs.uint_reg(arg1, static_cast<uint32_t>(value));
                    }
                    break;
                // }}}

                // ADD Commands {{{
                case ADD_R_R:
                    // reg[arg1] += reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg1) + _regs.uint_reg(arg2));
                    break;
                case ADD_R_R_R:
                    // reg[arg1] = reg[arg2] + reg[arg3]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg2) + _regs.uint_reg(arg3));
                    break;
                case ADD_R_R_I:
                    // reg[arg1] = reg[arg2] + arg3
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg2) + arg3);
                    break;
                case ADD_R_I:
                    // reg[arg1] += arg2
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg1) + arg2);
                    break;
                // }}}
                
                // ADDF Commands {{{
                case ADDF_R_R:
                    // reg[arg1] += reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg1) + _regs.float_reg(arg2));
                    break;
                case ADDF_R_R_R:
                    // reg[arg1] = reg[arg2] + reg[arg3]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg2) + _regs.float_reg(arg3));
                    break;
                case ADDF_R_R_I:
                    // reg[arg1] = reg[arg2] + arg3
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg2) + arg3);
                    break;
                case ADDF_R_I:
                    // reg[arg1] += arg2
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg1) + TO_FLOAT(arg2));
                    break;
                // }}}
                
                // SUB Commands {{{
                case SUB_R_R:
                    // reg[arg1] -= reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg1) - _regs.uint_reg(arg2));
                    break;
                case SUB_R_R_R:
                    // reg[arg1] = reg[arg2] - reg[arg3]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg2) - _regs.uint_reg(arg3));
                    break;
                case SUB_R_I_R:
                    // reg[arg1] = arg2 - reg[arg3]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.uint_reg(arg1, arg2 - _regs.uint_reg(arg3));
                    break;
                case SUB_R_R_I:
                    // reg[arg1] = reg[arg2] - arg3
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg2) - arg3);
                    break;
                case SUB_R_I:
                    // reg[arg1] -= arg2
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg1) - arg2);
                    break;
                // }}}

                // SUBF Commands {{{
                case SUBF_R_R:
                    // reg[arg1] -= reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg1) - _regs.float_reg(arg2));
                    break;
                case SUBF_R_R_R:
                    // reg[arg1] = reg[arg2] - reg[arg3]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg2) - _regs.float_reg(arg3));
                    break;
                case SUBF_R_I_R:
                    // reg[arg1] = arg2 - reg[arg3]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.float_reg(arg1, arg2 - _regs.float_reg(arg3));
                    break;
                case SUBF_R_R_I:
                    // reg[arg1] = reg[arg2] - arg3
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg2) - arg3);
                    break;
                case SUBF_R_I:
                    // reg[arg1] -= arg2
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg1) - TO_FLOAT(arg2));
                    break;
                // }}}
                
                // Stack commands {{{
                case PUSH_R:
                    // push reg[arg1]
                    push_stack(_regs.uint_reg(next_pc()));
                    break;
                case PUSH_I:
                    // push arg1
                    push_stack(next_pc());
                    break;
                case POP_R:
                    // reg[arg1] = pop
                    _regs.uint_reg(next_pc(), pop_stack());
                    break;
                
                case PUSHF_R:
                    // push reg[arg1]
                    {
                        float temp = _regs.float_reg(next_pc());
                        push_stack(TO_UINT(temp));
                    }
                    break;
                case PUSHF_I:
                    // push arg1
                    arg2 = next_pc();
                    push_stack(TO_UINT(arg2));
                    break;
                case POPF_R:
                    // reg[arg1] = pop
                    arg2 = pop_stack();
                    _regs.float_reg(next_pc(), TO_FLOAT(arg2));
                    break;
                // }}}

                // CMP commands {{{
                case CMP_R_R:
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = _regs.uint_reg(arg1) - _regs.uint_reg(arg2);
                    _regs.zero_flag(arg3 == 0);
                    _regs.neg_flag(arg3 & 0x80000000);
                    break;
                case CMP_R_I:
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = _regs.uint_reg(arg1) - arg2;
                    _regs.zero_flag(arg3 == 0);
                    _regs.neg_flag(arg3 & 0x80000000);
                    break;
                case CMP_I_R:
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = arg1 - _regs.uint_reg(arg2);
                    _regs.zero_flag(arg3 == 0);
                    _regs.neg_flag(arg3 & 0x80000000);
                    break;
                // }}}

                // CMPF commands {{{
                case CMPF_R_R:
                    arg1 = next_pc();
                    arg2 = next_pc();
                    {
                        float value = _regs.float_reg(arg1) - _regs.float_reg(arg2);
                        _regs.zero_flag(value == 0.0f);
                        _regs.neg_flag(value < 0.0f);
                    }
                    break;
                case CMPF_R_I:
                    arg1 = next_pc();
                    arg2 = next_pc();
                    {
                        float value = _regs.float_reg(arg1) - TO_FLOAT(arg2);
                        _regs.zero_flag(value == 0.0f);
                        _regs.neg_flag(value < 0.0f);
                    }
                    break;
                case CMPF_I_R:
                    arg1 = next_pc();
                    arg2 = next_pc();
                    {
                        float value = TO_FLOAT(arg1) - _regs.float_reg(arg2);
                        _regs.zero_flag(value == 0.0f);
                        _regs.neg_flag(value < 0.0f);
                    }
                    break;
                // }}}

                // Branching {{{
                
                // JMP commands {{{
                case JMP_R:
                    // pc = reg[arg1]
                    _regs.program_counter(_regs.uint_reg(next_pc()));
                    break;
                case JMP_I:
                    // pc = arg1
                    _regs.program_counter(next_pc());
                    break;
                // }}}
                
                // JNE commands {{{
                case JNE_R:
                    // pc = reg[arg1] if !zero_flag
                    arg1 = next_pc();
                    if (!_regs.zero_flag())
                    {
                        _regs.program_counter(_regs.uint_reg(arg1));
                    }
                    break;
                case JNE_I:
                    // pc = arg1 if !zero_flag
                    arg1 = next_pc();
                    if (!_regs.zero_flag())
                    {
                        _regs.program_counter(arg1);
                    }
                    break;
                // }}}
                
                // JEQ commands {{{
                case JEQ_R:
                    // pc = reg[arg1] if !zero_flag
                    arg1 = next_pc();
                    if (_regs.zero_flag())
                    {
                        _regs.program_counter(_regs.uint_reg(arg1));
                    }
                    break;
                case JEQ_I:
                    // pc = arg1 if zero_flag
                    arg1 = next_pc();
                    if (_regs.zero_flag())
                    {
                        _regs.program_counter(arg1);
                    }
                    break;
                // }}}
                
                // JLT commands {{{
                case JLT_R:
                    // pc = reg[arg1] if !zero_flag && neg_flag
                    arg1 = next_pc();
                    if (!_regs.zero_flag() && _regs.neg_flag())
                    {
                        _regs.program_counter(_regs.uint_reg(arg1));
                    }
                    break;
                case JLT_I:
                    // pc = arg1 if !zero_flag && neg_flag
                    arg1 = next_pc();
                    if (!_regs.zero_flag() && _regs.neg_flag())
                    {
                        _regs.program_counter(arg1);
                    }
                    break;
                // }}}
                
                // JLE commands {{{
                case JLE_R:
                    // pc = reg[arg1] if zero_flag || neg_flag
                    arg1 = next_pc();
                    if (_regs.zero_flag() || _regs.neg_flag())
                    {
                        _regs.program_counter(_regs.uint_reg(arg1));
                    }
                    break;
                case JLE_I:
                    // pc = arg1 if zero_flag || neg_flag
                    arg1 = next_pc();
                    if (_regs.zero_flag() || _regs.neg_flag())
                    {
                        _regs.program_counter(arg1);
                    }
                    break;
                // }}}
                
                // JGT commands {{{
                case JGT_R:
                    // pc = reg[arg1] if !zero_flag && !neg_flag
                    arg1 = next_pc();
                    if (!_regs.zero_flag() && !_regs.neg_flag())
                    {
                        _regs.program_counter(_regs.uint_reg(arg1));
                    }
                    break;
                case JGT_I:
                    // pc = reg[arg1] if !zero_flag && !neg_flag
                    arg1 = next_pc();
                    if (!_regs.zero_flag() && !_regs.neg_flag())
                    {
                        _regs.program_counter(arg1);
                    }
                    break;
                // }}}
                
                // JGE commands {{{
                case JGE_R:
                    // pc = reg[arg1] if zero_flag || !neg_flag
                    arg1 = next_pc();
                    if (_regs.zero_flag() || !_regs.neg_flag())
                    {
                        _regs.program_counter(_regs.uint_reg(arg1));
                    }
                    break;
                case JGE_I:
                    // pc = reg[arg1] if zero_flag || !neg_flag
                    arg1 = next_pc();
                    if (_regs.zero_flag() || !_regs.neg_flag())
                    {
                        _regs.program_counter(arg1);
                    }
                    break;
                // }}}
                
                // }}}

                // Debug commands {{{
                case PRINT_R:
                    arg1 = next_pc();
                    std::cout << "Reg " << arg1 << ": " << _regs.uint_reg(arg1) << std::endl;
                    break;
                // }}}

            }
        }
    }
}
