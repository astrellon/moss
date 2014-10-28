#include "registers.h"

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
        for (auto i = 0u; i < _int_regs.size(); i++)
        {
            _int_regs[i] = 0u;
        }
    }

    uint32_t Registers::flags() const
    {
        return _flags;
    }

    uint32_t Registers::num_int_reg() const
    {
        return static_cast<uint32_t>(_int_regs.size());
    }
    uint32_t Registers::int_reg(uint32_t index) const
    {
        return _int_regs[index];
    }
    void Registers::int_reg(uint32_t index, uint32_t value)
    {
        _int_regs[index] = value;
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

    uint32_t Registers::stack_pointer() const
    {
        return _stack_pointer;
    }
    uint32_t Registers::stack_pointer_push()
    {
        return ++_stack_pointer;
    }
    uint32_t Registers::stack_pointer_pop()
    {
        return --_stack_pointer;
    }
    void Registers::stack_pointer(uint32_t value)
    {
        _stack_pointer = value;
    }

    uint32_t Registers::data_pointer() const
    {
        return _data_pointer;
    }
    void Registers::data_pointer(uint32_t value)
    {
        _data_pointer = value;
    }
    
    uint32_t Registers::code_pointer() const
    {
        return _code_pointer;
    }
    void Registers::code_pointer(uint32_t value)
    {
        _code_pointer = value;
    }
    
    void Registers::to_stream(std::ostream &os) const
    {
        os << "Register: \n"
            "- PC: " << program_counter() << "\n"
            "- SP: " << stack_pointer() << "\n"
            "- Flags: " << flags() << "\n"
            ;
        for (auto i = 0; i < num_int_reg(); i++)
        {
            os << "- " << i << ": " << int_reg(i) << "\n";
        }
    }
}
