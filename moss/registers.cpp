#include "registers.h"

#include <iomanip>
#include <stdexcept>

#include "opcode.h"

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
            "- PC: " << program_counter() << "\n"
            "- SP: " << stack_pointer() << "\n"
            ;
        os << std::setw(5) << "Reg:" << std::setw(15) << "UINT" << std::setw(15) << "FLOAT\n";
        for (auto i = 0u; i < num_word_reg(); i++)
        {
            os << "- " << std::setw(2) << i << ": " << std::setw(14) << uint_reg(i) << std::setw(14) << float_reg(i) << "\n";
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
}
