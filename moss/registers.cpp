#include "registers.h"

#include <iomanip>
#include <stdexcept>

namespace moss
{
    Registers::Registers() :
        _flags(0),
        _stack_pointer(0),
        _min_stack_pointer(0),
        _max_stack_pointer(1024),
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
}
