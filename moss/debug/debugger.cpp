#include "debugger.h"

#include <iostream>

#include <moss/cpu/cpu_arm.h>
#include <moss/base/opcode.h>

namespace moss
{
    Debugger::Debugger(CpuArm *cpu, uint32_t program_offset) :
        _cpu(cpu),
        _program_offset(program_offset)
    {
         
    }

    void Debugger::on_break()
    {
        _cpu->debug_state(CpuArm::DEBUG_CONTINUE);
        std::cout << "BREAK START\n";
        _cpu->to_stream(std::cout);
        std::cout << "BREAK END\n";
    }


    void Debugger::breakpoint_line(const std::string &filename, uint32_t line_number, bool set)
    {
        auto program_index = _debug_data.data(filename, line_number);
        if (program_index == -1u)
        {
            std::cout << "Unable to set breakpoint for: " << filename << " @ " << line_number << "\n";
            return;
        }
        breakpoint(program_index, set);
    }
    bool Debugger::breakpoint_line(const std::string &filename, uint32_t line_number) const
    {
        auto program_index = _debug_data.data(filename, line_number);
        if (program_index == -1u)
        {
            std::cout << "Unable to get breakpoint from: " << filename << " @ " << line_number << "\n";
            return false;
        }
        return breakpoint(program_index);
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

    void Debugger::load_debug_data(std::istream &ss)
    {
        _debug_data.from_stream(ss);
    }
}
