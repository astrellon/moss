#include "registers.h"

#include <iomanip>
#include <stdexcept>

namespace moss
{
    Registers::Registers() :
        _flags(0),
        _stack_pointer(0),
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

    uint32_t Registers::flags() const
    {
        return _flags;
    }

    bool Registers::zero_flag() const
    {
        return (_flags & ZERO_FLAG) > 0;
    }
    void Registers::zero_flag(bool flag)
    {
        _flags = flag ? _flags | ZERO_FLAG : _flags & ~(ZERO_FLAG);
    }
    bool Registers::neg_flag() const
    {
        return (_flags & NEG_FLAG) > 0;
    }
    void Registers::neg_flag(bool flag)
    {
        _flags = flag ? _flags | NEG_FLAG : _flags & ~(NEG_FLAG);
    }

    uint32_t Registers::num_word_reg() const
    {
        return static_cast<uint32_t>(_word_regs.size());
    }
    // INT 
    int32_t Registers::int_reg(uint32_t index) const
    {
        return _word_regs.at(index).i;
    }
    void Registers::int_reg(uint32_t index, int32_t value)
    {
        if (index >= num_word_reg())
        {
            throw std::out_of_range("Out of register bounds");
        }
        _word_regs[index].i = value;
    }
    // UINT
    uint32_t Registers::uint_reg(uint32_t index) const
    {
        return _word_regs.at(index).u;
    }
    void Registers::uint_reg(uint32_t index, uint32_t value)
    {
        if (index >= num_word_reg())
        {
            throw std::out_of_range("Out of register bounds");
        }
        _word_regs[index].u = value;
    }
    // FLOAT
    float Registers::float_reg(uint32_t index) const
    {
        return _word_regs.at(index).f;
    }
    void Registers::float_reg(uint32_t index, float value)
    {
        if (index >= num_word_reg())
        {
            throw std::out_of_range("Out of register bounds");
        }
        _word_regs[index].f = value;
    }

    uint32_t Registers::program_counter() const
    {
        return _program_counter;
    }
    uint32_t Registers::program_counter_inc()
    {
        return _program_counter++;
    }
    void Registers::program_counter(uint32_t value)
    {
        _program_counter = value;
    }
    void Registers::change_program_counter(int32_t value)
    {
        _program_counter += value;
    }

    uint32_t Registers::stack_pointer() const
    {
        return _stack_pointer;
    }
    uint32_t Registers::stack_pointer_push()
    {
        return _stack_pointer++;
    }
    uint32_t Registers::stack_pointer_pop()
    {
        return --_stack_pointer;
    }
    void Registers::stack_pointer(uint32_t value)
    {
        _stack_pointer = value;
    }
    
    void Registers::to_stream(std::ostream &os) const
    {
        os << "Register: \n"
            "- PC: " << program_counter() << "\n"
            "- SP: " << stack_pointer() << "\n"
            "- Flags: " << flags() << "\n"
            ;
        os << std::setw(5) << "Reg:" << std::setw(15) << "UINT" << std::setw(15) << "FLOAT\n";
        for (auto i = 0; i < num_word_reg(); i++)
        {
            os << "- " << std::setw(2) << i << ": " << std::setw(14) << uint_reg(i) << std::setw(14) << float_reg(i) << "\n";
        }
    }
}
