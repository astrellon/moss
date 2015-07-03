#include "registers.h"

#include <iomanip>
#include <stdexcept>

#include <moss/base/opcode.h>

namespace moss
{
    Registers::Registers() :
        _flags(0),
        _stack_pointer(0),
        _min_stack_pointer(0),
        _max_stack_pointer(1024),
        _code_stack_pointer(0),
        _program_counter(0)
    {

    }

    void Registers::zero()
    {
        for (auto i = 0u; i < _word_regs.size(); i++)
        {
            _word_regs.at(i).u = 0u;
        }
    }

    void Registers::to_stream(std::ostream &os) const
    {
        os << std::dec << "Register: \n"
            "-  PC: " << program_counter() << "\n"
            "- CSP: " << code_stack_pointer() << "\n"
            "- DSP: " << stack_pointer() << "\n"
            ;
        os << std::setw(5) << "Reg:" << std::setw(15) << "INT" << std::setw(15) << "FLOAT\n";
        for (auto i = 0u; i < num_word_reg(); i++)
        {
            os << "- " << std::setw(2) << i << ": " << std::setw(14) << int_reg(i) << std::setw(14) << float_reg(i) << "\n";
        }
    }
	uint32_t Registers::named_register(uint32_t reg) const
	{
		switch (reg)
		{
			default:
			case Opcode::NAMED_UNKNOWN:
				std::cout << "Unknown named register: " << reg << "\n";
				return 0;
			case Opcode::STACK_POINTER:
				return _stack_pointer;
			case Opcode::CODE_STACK_POINTER:
				return _code_stack_pointer;
			case Opcode::PROGRAM_COUNTER:
				return _program_counter;
		}
	}
	void Registers::named_register(uint32_t reg, uint32_t value)
	{
		switch (reg)
		{
			default:
			case Opcode::NAMED_UNKNOWN:
				std::cout << "Unknown named register: " << reg << "\n";
				break;
			case Opcode::STACK_POINTER:
				_stack_pointer = value;
				break;
			case Opcode::CODE_STACK_POINTER:
				_code_stack_pointer = value;
				break;
			case Opcode::PROGRAM_COUNTER:
				_program_counter = value;
				break;
		}
	}
    
    Registers::Flags Registers::find_flag(const std::string &str)
    {
        auto find = s_names_to_flags.find(str);
        if (find != s_names_to_flags.end())
        {
            return find->second;
        }
        return Registers::FLAG_UNKNOWN;
    }
    std::string Registers::flag_name(Registers::Flags flag)
    {
        return flag_name(static_cast<uint32_t>(flag));
    }
    std::string Registers::flag_name(uint32_t flag)
    {
        for (auto iter = s_names_to_flags.begin(); iter != s_names_to_flags.end(); ++iter)
        {
            if (static_cast<uint32_t>(iter->second) == flag)
            {
                return iter->first;
            }
        }
        return std::string("Unknown flag");
    }

    // Names for flags. {{{
    // If a token that is expected to be an argument is one of these strings
    // it is now recognised as a flag value. This means that labels
    // cannot be one of these values.
    std::map<std::string, Registers::Flags> Registers::s_names_to_flags = {
        { "ZERO",     Registers::FLAG_ZERO },
        { "NEGATIVE", Registers::FLAG_NEGATIVE },
        { "CARRY",    Registers::FLAG_CARRY },
        { "OVERFLOW", Registers::FLAG_OVERFLOW },
        { "MMU",      Registers::FLAG_ENABLE_MMU }
    };
    // }}}

}
