#include "cpu_arm.h"

#include <iostream>

#include "memory.h"
#include "common.h"
#include "opcode.h"
#include "iperipheral.h"

namespace moss
{
    uint32_t CpuArm::s_int_bit_shift = (sizeof(uint32_t) << 3) - 1;
    CpuArm::CpuArm(uint32_t page_bit_size) :
        _running(false),
        _enable_mmu(false),
        _mmu(page_bit_size),
        _memory(nullptr)
    {
        _regs.zero();
    }
    CpuArm::~CpuArm()
    {
        stop();
    }

    Registers &CpuArm::registers()
    {
        return _regs;
    }
    const Registers &CpuArm::registers() const
    {
        return _regs;
    }

    bool CpuArm::enable_mmu() const
    {
        return _enable_mmu;
    }
    void CpuArm::enable_mmu(bool enable)
    {
        _enable_mmu = enable;
    }

    Mmu &CpuArm::mmu()
    {
        return _mmu;
    }
    const Mmu &CpuArm::mmu() const
    {
        return _mmu;
    }

    Memory *CpuArm::memory() const
    {
        return _memory;
    }
    void CpuArm::memory(Memory *memory)
    {
        _memory = memory;
        _mmu.memory(memory);
    }

    void CpuArm::run()
    {
        _running = true;
        do_run();
    }
    void CpuArm::stop()
    {
        _running = false;
    }
    bool CpuArm::is_running() const
    {
        return _running;
    }
    void CpuArm::install_peripheral(IPeripheral *perf)
    {
        _peripherals[0] = perf;
    }

    void CpuArm::to_stream(std::ostream &os) const
    {
        os << "CpuArm running: " << is_running() << "\n";
        _regs.to_stream(os);
    }

    void CpuArm::push_stack(uint32_t value)
    {
        _mmu.uint_data(_regs.stack_pointer_push(), value);
    }
    uint32_t CpuArm::pop_stack()
    {
        return _mmu.uint_data(_regs.stack_pointer_pop());
    }
    void CpuArm::push_stack_float(float value)
    {
        _mmu.float_data(_regs.stack_pointer_push(), value);
    }
    float CpuArm::pop_stack_float()
    {
        return _mmu.float_data(_regs.stack_pointer_pop());
    }

    void CpuArm::do_run()
    {
        float farg1 = 0.0f;
        float farg2 = 0.0f;
        int32_t iarg1 = 0;
        int32_t iarg2 = 0;
        uint32_t arg1 = 0u;
        uint32_t arg2 = 0u;
        uint32_t arg3 = 0u;
        while (_running)
        {
            uint32_t opcode = next_pc_uint();
            bool meets_condition = opcode < Opcode::COND_ANY;
            if (!meets_condition)
            {
                // Change to switch case with the conditional flags, need
                // a meets_condition for each code path.
                uint32_t conds = opcode & Opcode::COND_ALL;
                opcode &= ~(Opcode::COND_ALL);
                
                switch (conds)
                {
                    case Opcode::COND_EQ:
                        meets_condition = _regs.zero_flag();
                        break;
                    case Opcode::COND_NE:
                        meets_condition = !_regs.zero_flag();
                        break;
                    case Opcode::COND_LT:
                        meets_condition = _regs.neg_flag() && !_regs.zero_flag();
                        break;
                    case Opcode::COND_LE:
                        meets_condition = _regs.neg_flag() || _regs.zero_flag();
                        break;
                    case Opcode::COND_GT:
                        meets_condition = !_regs.neg_flag() && !_regs.zero_flag();
                        break;;
                    case Opcode::COND_GE:
                        meets_condition = !_regs.neg_flag() || _regs.zero_flag();
                        break;
                    default:
                    case 0:
                        break;
                }
            }

            switch (opcode)
            {
                // Error or Halt {{{
                    default:
                    std::cout << "Unknown OPCODE: " << opcode << " @ PC " << 
                        (_regs.program_counter() - 1) << std::endl;
                case Opcode::HALT:
                    std::cout << "Halting." << std::endl;
                    stop();
                    break;
                // }}}

                // MOV Commands {{{
                case Opcode::MOV_R_R:
                    // reg[arg1] = reg[arg2]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg2));
                    }
                    break;
                case Opcode::MOV_R_I:
                    // reg[arg1] = arg2
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, arg2);
                    }
                    break;

                case Opcode::MOV_M_R:
                    // mem[reg[arg1]] = reg[arg2]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _mmu.uint_data(_regs.uint_reg(arg1), _regs.uint_reg(arg2));
                    }
                    break;
                case Opcode::MOV_R_M:
                    // reg[arg1] = mem[reg[arg2]]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _mmu.uint_data(_regs.uint_reg(arg2)));
                    }
                    break;
                case Opcode::MOV_M_I:
                    // mem[reg[arg1]] = arg2
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _mmu.uint_data(_regs.uint_reg(arg1), arg2);
                    }
                    break;
                case Opcode::MOV_M_M:
                    // mem[reg[arg1]] = mem[reg[arg2]]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _mmu.uint_data(_regs.uint_reg(arg1), _mmu.uint_data(_regs.uint_reg(arg2)));
                    }
                    break;

                case Opcode::MOV_R_F:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, static_cast<uint32_t>(_regs.flag(arg2)));
                    }
                    break;
                case Opcode::MOV_F_R:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.flag(arg1, _regs.uint_reg(arg2) > 0);
                    }
                    break;
                case Opcode::MOV_F_I:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.flag(arg1, arg2 > 0);
                    }
                    break;
                // }}}

                // Unit Converstions {{{
                case Opcode::UINT_FLOAT_R:
                    arg1 = next_pc_uint();
                    if (meets_condition)
                    {
                        uint32_t value = _regs.uint_reg(arg1);
                        _regs.float_reg(arg1, static_cast<float>(value));
                    }
                    break;
                case Opcode::UINT_FLOAT_R_R:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        uint32_t value = _regs.uint_reg(arg2);
                        _regs.float_reg(arg1, static_cast<float>(value));
                    }
                    break;
                case Opcode::FLOAT_UINT_R:
                    arg1 = next_pc_uint();
                    if (meets_condition)
                    {
                        float value = _regs.float_reg(arg1);
                        _regs.uint_reg(arg1, static_cast<uint32_t>(value));
                    }
                    break;
                case Opcode::FLOAT_UINT_R_R:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        float value = _regs.float_reg(arg2);
                        _regs.uint_reg(arg1, static_cast<uint32_t>(value));
                    }
                    break;
                // }}}

                // Stack commands {{{
                case Opcode::PUSH_R:
                    // push reg[arg1]
                    arg1 = next_pc_uint();
                    if (meets_condition)
                    {
                        push_stack(_regs.uint_reg(arg1));
                    }
                    break;
                case Opcode::PUSH_I:
                    // push arg1
                    arg1 = next_pc_uint();
                    if (meets_condition)
                    {
                        push_stack(arg1);
                    }
                    break;
                case Opcode::POP_R:
                    // reg[arg1] = pop
                    arg1 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, pop_stack());
                    }
                    break;
                // }}}

                // CMP commands {{{
                case Opcode::CMP_R_R:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        arg3 = _regs.uint_reg(arg1) - _regs.uint_reg(arg2);
                        _regs.zero_flag(arg3 == 0);
                        _regs.neg_flag(arg3 & 0x80000000);
                    }
                    break;
                case Opcode::CMP_R_I:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        arg3 = _regs.uint_reg(arg1) - arg2;
                        _regs.zero_flag(arg3 == 0);
                        _regs.neg_flag(arg3 & 0x80000000);
                    }
                    break;
                case Opcode::CMP_I_R:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        arg3 = arg1 - _regs.uint_reg(arg2);
                        _regs.zero_flag(arg3 == 0);
                        _regs.neg_flag(arg3 & 0x80000000);
                    }
                    break;
                // }}}

                // CMPF commands {{{
                case Opcode::CMPF_R_R:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        float value = _regs.float_reg(arg1) - _regs.float_reg(arg2);
                        _regs.zero_flag(value == 0.0f);
                        _regs.neg_flag(value < 0.0f);
                    }
                    break;
                case Opcode::CMPF_R_I:
                    arg1 = next_pc_uint();
                    farg2 = next_pc_float();
                    if (meets_condition)
                    {
                        float value = _regs.float_reg(arg1) - farg2;
                        _regs.zero_flag(value == 0.0f);
                        _regs.neg_flag(value < 0.0f);
                    }
                    break;
                case Opcode::CMPF_I_R:
                    farg1 = next_pc_float();
                    arg2 = next_pc_uint();
                    if (meets_condition)
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
                    arg1 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.program_counter(_regs.uint_reg(arg1));
                    }
                    break;
                case Opcode::JMP_I:
                    // pc += arg1
                    arg1 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.change_program_counter(arg1);
                    }
                    break;
                // }}}
                
                // }}}

                // ADD Commands {{{
                case Opcode::ADD_R_R:
                    // reg[arg1] += reg[arg2]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg1) + _regs.uint_reg(arg2));
                    }
                    break;
                case Opcode::ADD_R_R_R:
                    // reg[arg1] = reg[arg2] + reg[arg3]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg2) + _regs.uint_reg(arg3));
                    }
                    break;
                case Opcode::ADD_R_R_I:
                    // reg[arg1] = reg[arg2] + arg3
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg2) + arg3);
                    }
                    break;
                case Opcode::ADD_R_I:
                    // reg[arg1] += arg2
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg1) + arg2);
                    }
                    break;
                // }}}
                
                // ADDF Commands {{{
                case Opcode::ADDF_R_R:
                    // reg[arg1] += reg[arg2]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg1) + _regs.float_reg(arg2));
                    }
                    break;
                case Opcode::ADDF_R_R_R:
                    // reg[arg1] = reg[arg2] + reg[arg3]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg2) + _regs.float_reg(arg3));
                    }
                    break;
                case Opcode::ADDF_R_R_I:
                    // reg[arg1] = reg[arg2] + arg3
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg2) + arg3);
                    }
                    break;
                case Opcode::ADDF_R_I:
                    // reg[arg1] += arg2
                    arg1 = next_pc_uint();
                    farg2 = next_pc_float();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg1) + arg2);
                    }
                    break;
                // }}}
                
                // SUB Commands {{{
                case Opcode::SUB_R_R:
                    // reg[arg1] -= reg[arg2]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg1) - _regs.uint_reg(arg2));
                    }
                    break;
                case Opcode::SUB_R_R_R:
                    // reg[arg1] = reg[arg2] - reg[arg3]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg2) - _regs.uint_reg(arg3));
                    }
                    break;
                case Opcode::SUB_R_I_R:
                    // reg[arg1] = arg2 - reg[arg3]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, arg2 - _regs.uint_reg(arg3));
                    }
                    break;
                case Opcode::SUB_R_R_I:
                    // reg[arg1] = reg[arg2] - arg3
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg2) - arg3);
                    }
                    break;
                case Opcode::SUB_R_I:
                    // reg[arg1] -= arg2
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg1) - arg2);
                    }
                    break;
                // }}}

                // SUBF Commands {{{
                case Opcode::SUBF_R_R:
                    // reg[arg1] -= reg[arg2]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg1) - _regs.float_reg(arg2));
                    }
                    break;
                case Opcode::SUBF_R_R_R:
                    // reg[arg1] = reg[arg2] - reg[arg3]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg2) - _regs.float_reg(arg3));
                    }
                    break;
                case Opcode::SUBF_R_I_R:
                    // reg[arg1] = arg2 - reg[arg3]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, arg2 - _regs.float_reg(arg3));
                    }
                    break;
                case Opcode::SUBF_R_R_I:
                    // reg[arg1] = reg[arg2] - arg3
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg2) - arg3);
                    }
                    break;
                case Opcode::SUBF_R_I:
                    // reg[arg1] -= arg2
                    arg1 = next_pc_uint();
                    farg2 = next_pc_float();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg1) - arg2);
                    }
                    break;
                // }}}

                // INC/DEC Commands {{{
                case Opcode::INC_R:
                    // reg[arg1] += 1
                    arg1 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg1) + 1);
                    }
                    break;
                case Opcode::INCF_R:
                    // reg[arg1] += 1.0f
                    arg1 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg1) + 1.0f);
                    }
                    break;
                case Opcode::DEC_R:
                    // reg[arg1] -= 1
                    arg1 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg1) - 1);
                    }
                    break;
                case Opcode::DECF_R:
                    // reg[arg1] -= 1.0f
                    arg1 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg1) - 1.0f);
                    }
                    break;
                // }}}
                
                // MUL Commands {{{
                case Opcode::MUL_R_R:
                    // reg[arg1] *= reg[arg2]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg1) * _regs.uint_reg(arg2));
                    }
                    break;
                case Opcode::MUL_R_R_R:
                    // reg[arg1] = reg[arg2] * reg[arg3]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg2) * _regs.uint_reg(arg3));
                    }
                    break;
                case Opcode::MUL_R_R_I:
                    // reg[arg1] = reg[arg2] * arg3
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg2) * arg3);
                    }
                    break;
                case Opcode::MUL_R_I:
                    // reg[arg1] *= arg2
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg1) * arg2);
                    }
                    break;
                // }}}
                
                // MULF Commands {{{
                case Opcode::MULF_R_R:
                    // reg[arg1] *= reg[arg2]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg1) * _regs.float_reg(arg2));
                    }
                    break;
                case Opcode::MULF_R_R_R:
                    // reg[arg1] = reg[arg2] * reg[arg3]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg2) * _regs.float_reg(arg3));
                    }
                    break;
                case Opcode::MULF_R_R_I:
                    // reg[arg1] = reg[arg2] * arg3
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg2) * arg3);
                    }
                    break;
                case Opcode::MULF_R_I:
                    // reg[arg1] *= arg2
                    arg1 = next_pc_uint();
                    farg2 = next_pc_float();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg1) * arg2);
                    }
                    break;
                // }}}
                
                // DIV Commands {{{
                case Opcode::DIV_R_R:
                    // reg[arg1] /= reg[arg2]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg1) / _regs.uint_reg(arg2));
                    }
                    break;
                case Opcode::DIV_R_R_R:
                    // reg[arg1] = reg[arg2] / reg[arg3]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg2) / _regs.uint_reg(arg3));
                    }
                    break;
                case Opcode::DIV_R_I_R:
                    // reg[arg1] = arg2 / reg[arg3]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, arg2 / _regs.uint_reg(arg3));
                    }
                    break;
                case Opcode::DIV_R_R_I:
                    // reg[arg1] = reg[arg2] / arg3
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg2) / arg3);
                    }
                    break;
                case Opcode::DIV_R_I:
                    // reg[arg1] /= arg2
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg1) / arg2);
                    }
                    break;
                // }}}

                // DIVF Commands {{{
                case Opcode::DIVF_R_R:
                    // reg[arg1] /= reg[arg2]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg1) / _regs.float_reg(arg2));
                    }
                    break;
                case Opcode::DIVF_R_R_R:
                    // reg[arg1] = reg[arg2] / reg[arg3]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg2) / _regs.float_reg(arg3));
                    }
                    break;
                case Opcode::DIVF_R_I_R:
                    // reg[arg1] = arg2 / reg[arg3]
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, arg2 / _regs.float_reg(arg3));
                    }
                    break;
                case Opcode::DIVF_R_R_I:
                    // reg[arg1] = reg[arg2] / arg3
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg2) / arg3);
                    }
                    break;
                case Opcode::DIVF_R_I:
                    // reg[arg1] /= arg2
                    arg1 = next_pc_uint();
                    farg2 = next_pc_float();
                    if (meets_condition)
                    {
                        _regs.float_reg(arg1, _regs.float_reg(arg1) / arg2);
                    }
                    break;
                // }}}

                // Rotate Right/Left Commands {{{
                case Opcode::ROR_R:
                    arg1 = next_pc_uint();
                    arg2 = _regs.uint_reg(arg1);
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, (arg2 << 1) | (arg2 >> s_int_bit_shift));
                    }
                    break;
                case Opcode::ROR_R_R:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = _regs.uint_reg(arg2);
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, (arg3 << 1) | (arg3 >> s_int_bit_shift));
                    }
                    break;
                case Opcode::ROL_R:
                    arg1 = next_pc_uint();
                    arg2 = _regs.uint_reg(arg1);
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, (arg2 >> 1) | (arg2 << s_int_bit_shift));
                    }
                    break;
                case Opcode::ROL_R_R:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = _regs.uint_reg(arg2);
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, (arg3 >> 1) | (arg3 << s_int_bit_shift));
                    }
                    break;
                // }}}

                // Shift Right/Left Commands {{{
                case Opcode::SHR_R:
                    arg1 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg1) << 1);
                    }
                    break;
                case Opcode::SHR_R_R:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg2) << 1);
                    }
                    break;
                case Opcode::SHL_R:
                    arg1 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg1) >> 1);
                    }
                    break;
                case Opcode::SHL_R_R:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _regs.uint_reg(arg2) >> 1);
                    }
                    break;
                // }}}

                // Peripherals commands {{{
                
                // SEND {{{
                case Opcode::SEND_I_I:
                    // peripheral[arg1](arg2)
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _peripherals[arg1]->send_command(arg2);
                    }
                    break;
                case Opcode::SEND_R_I:
                    // peripheral[reg[arg1]](arg2)
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _peripherals[_regs.uint_reg(arg1)]->send_command(arg2);
                    }
                    break;
                case Opcode::SEND_I_R:
                    // peripheral[arg1](reg[arg2])
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _peripherals[arg1]->send_command(_regs.uint_reg(arg2));
                    }
                    break;
                case Opcode::SEND_R_R:
                    // peripheral[reg[arg1]](reg[arg2])
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    if (meets_condition)
                    {
                        _peripherals[_regs.uint_reg(arg1)]->send_command(_regs.uint_reg(arg2));
                    }
                    break;

                case Opcode::SEND_R_I_I:
                    // reg[arg1] = peripheral[arg2](arg3)
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _peripherals[arg2]->send_command(arg3));
                    }
                    break;
                case Opcode::SEND_R_R_I:
                    // reg[arg1] = peripheral[reg[arg2]](arg3)
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _peripherals[_regs.uint_reg(arg2)]->send_command(arg3));
                    }
                    break;
                case Opcode::SEND_R_I_R:
                    // reg[arg1] = peripheral[arg2](reg[arg3])
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _peripherals[arg2]->send_command(_regs.uint_reg(arg3)));
                    }
                    break;
                case Opcode::SEND_R_R_R:
                    // reg[arg1] = peripheral[reg[arg2]](reg[arg3])
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _regs.uint_reg(arg1, _peripherals[_regs.uint_reg(arg2)]->send_command(_regs.uint_reg(arg3)));
                    }
                    break;

                // }}}

                // ASSIGN {{{
                case Opcode::ASSIGN_R_R_R:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _peripherals[_regs.uint_reg(arg1)]->assign_memory(_memory, _regs.uint_reg(arg2), _regs.uint_reg(arg3)); 
                    }
                    break;
                case Opcode::ASSIGN_R_R_I:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _peripherals[_regs.uint_reg(arg1)]->assign_memory(_memory, _regs.uint_reg(arg2), arg3); 
                    }
                    break;
                case Opcode::ASSIGN_R_I_R:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _peripherals[_regs.uint_reg(arg1)]->assign_memory(_memory, arg2, _regs.uint_reg(arg3)); 
                    }
                    break;
                case Opcode::ASSIGN_R_I_I:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _peripherals[_regs.uint_reg(arg1)]->assign_memory(_memory, arg2, arg3); 
                    }
                    break;

                case Opcode::ASSIGN_I_R_R:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _peripherals[arg1]->assign_memory(_memory, _regs.uint_reg(arg2), _regs.uint_reg(arg3)); 
                    }
                    break;
                case Opcode::ASSIGN_I_R_I:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _peripherals[arg1]->assign_memory(_memory, _regs.uint_reg(arg2), arg3); 
                    }
                    break;
                case Opcode::ASSIGN_I_I_R:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _peripherals[arg1]->assign_memory(_memory, arg2, _regs.uint_reg(arg3)); 
                    }
                    break;
                case Opcode::ASSIGN_I_I_I:
                    arg1 = next_pc_uint();
                    arg2 = next_pc_uint();
                    arg3 = next_pc_uint();
                    if (meets_condition)
                    {
                        _peripherals[arg1]->assign_memory(_memory, arg2, arg3); 
                    }
                    break;
                // }}}
                
                // }}}

                // Debug commands {{{
                case Opcode::PRINT_R:
                    arg1 = next_pc_uint();
                    std::cout << "Reg " << arg1 << ": " << _regs.uint_reg(arg1) << std::endl;
                    break;
                // }}}

            }
        }
    }
}
