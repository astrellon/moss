#include "cpu.h"

#include <iostream>

#include "memory.h"
#include "common.h"
#include "opcode.h"

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
    float Cpu::next_pc_float()
    {
        return _mmu.float_data(_regs.program_counter_inc());
    }

    void Cpu::push_stack(uint32_t value)
    {
        _mmu.uint_data(_regs.stack_pointer_push(), value);
    }
    uint32_t Cpu::pop_stack()
    {
        return _mmu.uint_data(_regs.stack_pointer_pop());
    }
    void Cpu::push_stack_float(float value)
    {
        _mmu.float_data(_regs.stack_pointer_push(), value);
    }
    float Cpu::pop_stack_float()
    {
        return _mmu.float_data(_regs.stack_pointer_pop());
    }

    void Cpu::do_run()
    {
        float farg1 = 0.0f;
        float farg2 = 0.0f;
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
                case Opcode::HALT:
                    stop();
                    break;
                // }}}

                // MOV Commands {{{
                case Opcode::MOV_R_R:
                    // reg[arg1] = reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg2));
                    break;
                case Opcode::MOV_R_I:
                    // reg[arg1] = arg2
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.uint_reg(arg1, arg2);
                    break;
                case Opcode::MOV_M_R:
                    // mem[reg[arg1]] = reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _mmu.uint_data(_regs.uint_reg(arg1), _regs.uint_reg(arg2));
                    break;
                case Opcode::MOV_R_M:
                    // reg[arg1] = mem[reg[arg2]]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.uint_reg(arg1, _mmu.uint_data(_regs.uint_reg(arg2)));
                    break;
                case Opcode::MOV_M_I:
                    // mem[reg[arg1]] = arg2
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _mmu.uint_data(_regs.uint_reg(arg1), arg2);
                    break;
                case Opcode::MOV_M_M:
                    // mem[reg[arg1]] = mem[reg[arg2]]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _mmu.uint_data(_regs.uint_reg(arg1), _mmu.uint_data(_regs.uint_reg(arg2)));
                    break;
                // }}}
                
                    /*
                // MOVF Commands {{{
                case Opcode::MOVF_R_R:
                    // reg[arg1] = reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg2));
                    break;
                case Opcode::MOVF_R_I:
                    // reg[arg1] = arg2
                    arg1 = next_pc();
                    farg2 = next_pc_float();
                    _regs.float_reg(arg1, farg2);
                    break;
                case Opcode::MOVF_M_R:
                    // mem[reg[arg1]] = reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _mmu.float_data(_regs.float_reg(arg1), _regs.float_reg(arg2));
                    break;
                case Opcode::MOVF_R_M:
                    // reg[arg1] = mem[reg[arg2]]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.float_reg(arg1, _mmu.float_data(_regs.float_reg(arg2)));
                    break;
                case Opcode::MOVF_M_I:
                    // mem[reg[arg1]] = arg2
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _mmu.float_data(_regs.float_reg(arg1), arg2);
                    break;
                case Opcode::MOVF_M_M:
                    // mem[reg[arg1]] = mem[reg[arg2]]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _mmu.float_data(_regs.float_reg(arg1), _mmu.float_data(_regs.float_reg(arg2)));
                    break;
                // }}}
                */

                // Unit Converstions {{{
                case Opcode::UINT_FLOAT_R:
                    arg1 = next_pc();
                    {
                        uint32_t value = _regs.uint_reg(arg1);
                        _regs.float_reg(arg1, static_cast<float>(value));
                    }
                    break;
                case Opcode::UINT_FLOAT_R_R:
                    arg1 = next_pc();
                    arg2 = next_pc();
                    {
                        uint32_t value = _regs.uint_reg(arg2);
                        _regs.float_reg(arg1, static_cast<float>(value));
                    }
                    break;
                case Opcode::FLOAT_UINT_R:
                    arg1 = next_pc();
                    {
                        float value = _regs.float_reg(arg1);
                        _regs.uint_reg(arg1, static_cast<uint32_t>(value));
                    }
                    break;
                case Opcode::FLOAT_UINT_R_R:
                    arg1 = next_pc();
                    arg2 = next_pc();
                    {
                        float value = _regs.float_reg(arg2);
                        _regs.uint_reg(arg1, static_cast<uint32_t>(value));
                    }
                    break;
                // }}}

                // ADD Commands {{{
                case Opcode::ADD_R_R:
                    // reg[arg1] += reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg1) + _regs.uint_reg(arg2));
                    break;
                case Opcode::ADD_R_R_R:
                    // reg[arg1] = reg[arg2] + reg[arg3]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg2) + _regs.uint_reg(arg3));
                    break;
                case Opcode::ADD_R_R_I:
                    // reg[arg1] = reg[arg2] + arg3
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg2) + arg3);
                    break;
                case Opcode::ADD_R_I:
                    // reg[arg1] += arg2
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg1) + arg2);
                    break;
                // }}}
                
                // ADDF Commands {{{
                case Opcode::ADDF_R_R:
                    // reg[arg1] += reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg1) + _regs.float_reg(arg2));
                    break;
                case Opcode::ADDF_R_R_R:
                    // reg[arg1] = reg[arg2] + reg[arg3]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg2) + _regs.float_reg(arg3));
                    break;
                case Opcode::ADDF_R_R_I:
                    // reg[arg1] = reg[arg2] + arg3
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg2) + arg3);
                    break;
                case Opcode::ADDF_R_I:
                    // reg[arg1] += arg2
                    arg1 = next_pc();
                    farg2 = next_pc_float();
                    _regs.float_reg(arg1, _regs.float_reg(arg1) + arg2);
                    break;
                // }}}
                
                // SUB Commands {{{
                case Opcode::SUB_R_R:
                    // reg[arg1] -= reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg1) - _regs.uint_reg(arg2));
                    break;
                case Opcode::SUB_R_R_R:
                    // reg[arg1] = reg[arg2] - reg[arg3]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg2) - _regs.uint_reg(arg3));
                    break;
                case Opcode::SUB_R_I_R:
                    // reg[arg1] = arg2 - reg[arg3]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.uint_reg(arg1, arg2 - _regs.uint_reg(arg3));
                    break;
                case Opcode::SUB_R_R_I:
                    // reg[arg1] = reg[arg2] - arg3
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg2) - arg3);
                    break;
                case Opcode::SUB_R_I:
                    // reg[arg1] -= arg2
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.uint_reg(arg1, _regs.uint_reg(arg1) - arg2);
                    break;
                // }}}

                // SUBF Commands {{{
                case Opcode::SUBF_R_R:
                    // reg[arg1] -= reg[arg2]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg1) - _regs.float_reg(arg2));
                    break;
                case Opcode::SUBF_R_R_R:
                    // reg[arg1] = reg[arg2] - reg[arg3]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg2) - _regs.float_reg(arg3));
                    break;
                case Opcode::SUBF_R_I_R:
                    // reg[arg1] = arg2 - reg[arg3]
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.float_reg(arg1, arg2 - _regs.float_reg(arg3));
                    break;
                case Opcode::SUBF_R_R_I:
                    // reg[arg1] = reg[arg2] - arg3
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = next_pc();
                    _regs.float_reg(arg1, _regs.float_reg(arg2) - arg3);
                    break;
                case Opcode::SUBF_R_I:
                    // reg[arg1] -= arg2
                    arg1 = next_pc();
                    farg2 = next_pc_float();
                    _regs.float_reg(arg1, _regs.float_reg(arg1) - arg2);
                    break;
                // }}}
                
                // Stack commands {{{
                case Opcode::PUSH_R:
                    // push reg[arg1]
                    push_stack(_regs.uint_reg(next_pc()));
                    break;
                case Opcode::PUSH_I:
                    // push arg1
                    push_stack(next_pc());
                    break;
                case Opcode::POP_R:
                    // reg[arg1] = pop
                    _regs.uint_reg(next_pc(), pop_stack());
                    break;
                
                    /*
                case Opcode::PUSHF_R:
                    // push reg[arg1]
                    push_stack_float(_regs.float_reg(next_pc()));
                    break;
                case Opcode::PUSHF_I:
                    // push arg1
                    push_stack_float(next_pc_float());
                    break;
                case Opcode::POPF_R:
                    // reg[arg1] = pop
                    farg2 = pop_stack_float();
                    _regs.float_reg(next_pc(), arg2);
                    break;
                    */
                // }}}

                // CMP commands {{{
                case Opcode::CMP_R_R:
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = _regs.uint_reg(arg1) - _regs.uint_reg(arg2);
                    _regs.zero_flag(arg3 == 0);
                    _regs.neg_flag(arg3 & 0x80000000);
                    break;
                case Opcode::CMP_R_I:
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = _regs.uint_reg(arg1) - arg2;
                    _regs.zero_flag(arg3 == 0);
                    _regs.neg_flag(arg3 & 0x80000000);
                    break;
                case Opcode::CMP_I_R:
                    arg1 = next_pc();
                    arg2 = next_pc();
                    arg3 = arg1 - _regs.uint_reg(arg2);
                    _regs.zero_flag(arg3 == 0);
                    _regs.neg_flag(arg3 & 0x80000000);
                    break;
                // }}}

                // CMPF commands {{{
                case Opcode::CMPF_R_R:
                    arg1 = next_pc();
                    arg2 = next_pc();
                    {
                        float value = _regs.float_reg(arg1) - _regs.float_reg(arg2);
                        _regs.zero_flag(value == 0.0f);
                        _regs.neg_flag(value < 0.0f);
                    }
                    break;
                case Opcode::CMPF_R_I:
                    arg1 = next_pc();
                    farg2 = next_pc_float();
                    {
                        float value = _regs.float_reg(arg1) - farg2;
                        _regs.zero_flag(value == 0.0f);
                        _regs.neg_flag(value < 0.0f);
                    }
                    break;
                case Opcode::CMPF_I_R:
                    farg1 = next_pc_float();
                    arg2 = next_pc();
                    {
                        float value = farg1 - _regs.float_reg(arg2);
                        _regs.zero_flag(value == 0.0f);
                        _regs.neg_flag(value < 0.0f);
                    }
                    break;
                // }}}

                // Branching {{{
                
                // JMP commands {{{
                case Opcode::JMP_R:
                    // pc = reg[arg1]
                    _regs.program_counter(_regs.uint_reg(next_pc()));
                    break;
                case Opcode::JMP_I:
                    // pc = arg1
                    _regs.program_counter(next_pc());
                    break;
                // }}}
                
                // JNE commands {{{
                case Opcode::JNE_R:
                    // pc = reg[arg1] if !zero_flag
                    arg1 = next_pc();
                    if (!_regs.zero_flag())
                    {
                        _regs.program_counter(_regs.uint_reg(arg1));
                    }
                    break;
                case Opcode::JNE_I:
                    // pc = arg1 if !zero_flag
                    arg1 = next_pc();
                    if (!_regs.zero_flag())
                    {
                        _regs.program_counter(arg1);
                    }
                    break;
                // }}}
                
                // JEQ commands {{{
                case Opcode::JEQ_R:
                    // pc = reg[arg1] if !zero_flag
                    arg1 = next_pc();
                    if (_regs.zero_flag())
                    {
                        _regs.program_counter(_regs.uint_reg(arg1));
                    }
                    break;
                case Opcode::JEQ_I:
                    // pc = arg1 if zero_flag
                    arg1 = next_pc();
                    if (_regs.zero_flag())
                    {
                        _regs.program_counter(arg1);
                    }
                    break;
                // }}}
                
                // JLT commands {{{
                case Opcode::JLT_R:
                    // pc = reg[arg1] if !zero_flag && neg_flag
                    arg1 = next_pc();
                    if (!_regs.zero_flag() && _regs.neg_flag())
                    {
                        _regs.program_counter(_regs.uint_reg(arg1));
                    }
                    break;
                case Opcode::JLT_I:
                    // pc = arg1 if !zero_flag && neg_flag
                    arg1 = next_pc();
                    if (!_regs.zero_flag() && _regs.neg_flag())
                    {
                        _regs.program_counter(arg1);
                    }
                    break;
                // }}}
                
                // JLE commands {{{
                case Opcode::JLE_R:
                    // pc = reg[arg1] if zero_flag || neg_flag
                    arg1 = next_pc();
                    if (_regs.zero_flag() || _regs.neg_flag())
                    {
                        _regs.program_counter(_regs.uint_reg(arg1));
                    }
                    break;
                case Opcode::JLE_I:
                    // pc = arg1 if zero_flag || neg_flag
                    arg1 = next_pc();
                    if (_regs.zero_flag() || _regs.neg_flag())
                    {
                        _regs.program_counter(arg1);
                    }
                    break;
                // }}}
                
                // JGT commands {{{
                case Opcode::JGT_R:
                    // pc = reg[arg1] if !zero_flag && !neg_flag
                    arg1 = next_pc();
                    if (!_regs.zero_flag() && !_regs.neg_flag())
                    {
                        _regs.program_counter(_regs.uint_reg(arg1));
                    }
                    break;
                case Opcode::JGT_I:
                    // pc = reg[arg1] if !zero_flag && !neg_flag
                    arg1 = next_pc();
                    if (!_regs.zero_flag() && !_regs.neg_flag())
                    {
                        _regs.program_counter(arg1);
                    }
                    break;
                // }}}
                
                // JGE commands {{{
                case Opcode::JGE_R:
                    // pc = reg[arg1] if zero_flag || !neg_flag
                    arg1 = next_pc();
                    if (_regs.zero_flag() || !_regs.neg_flag())
                    {
                        _regs.program_counter(_regs.uint_reg(arg1));
                    }
                    break;
                case Opcode::JGE_I:
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
                case Opcode::PRINT_R:
                    arg1 = next_pc();
                    std::cout << "Reg " << arg1 << ": " << _regs.uint_reg(arg1) << std::endl;
                    break;
                // }}}

            }
        }
    }
}
