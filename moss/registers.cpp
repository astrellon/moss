#include "registers.h"

namespace moss
{
    Registers::Registers() :
        _flags(0)
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
    
    void Registers::to_stream(std::ostream &os) const
    {
        os << "Flags: " << flags() << "\n";
        for (auto i = 0; i < num_int_reg(); i++)
        {
            os << "- " << i << ": " << int_reg(i) << "\n";
        }
    }
}
