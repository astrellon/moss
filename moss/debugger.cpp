#include "debugger.h"

#include <iostream>

#include "cpu_arm.h"
#include "opcode.h"

namespace moss
{
    Debugger::Debugger(CpuArm *cpu, uint32_t program_offset) :
        _cpu(cpu),
        _program_offset(program_offset)
    {
         
    }

    void Debugger::on_break()
    {
        _cpu->to_stream(std::cout);
    }


    void Debugger::breakpoint_line(const std::string &filename, uint32_t line_number, bool set)
    {

    }
    void Debugger::breakpoint(uint32_t program_index, bool set)
    {
        auto opcode = _cpu->mmu().uint_data(program_index + _program_offset);
        opcode = set ? opcode | Opcode::COND_BREAK : opcode & (~Opcode::COND_BREAK);
        _cpu->mmu().uint_data(program_index + _program_offset, opcode);
    }
    bool Debugger::breakpoint(uint32_t program_index) const
    {
        auto opcode = _cpu->mmu().uint_data(program_index + _program_offset);
        return (opcode & Opcode::COND_BREAK) > 0;
    }

}
