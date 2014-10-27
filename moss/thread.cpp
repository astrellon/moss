#include "thread.h"

namespace moss
{
    Thread::Thread() :
        _program_counter(0),
        _stack_pointer(0)
    {
        _regs.zero();
    }
    Thread::~Thread()
    {

    }

    uint32_t Thread::program_counter() const
    {
        return _program_counter;
    }
    uint32_t Thread::program_counter_inc()
    {
        return _program_counter++;
    }
    void Thread::program_counter(uint32_t counter)
    {
        _program_counter = counter;
    }

    uint32_t Thread::stack_pointer() const
    {
        return _stack_pointer;
    }

    Registers &Thread::registers()
    {
        return _regs;
    }
    const Registers &Thread::registers() const 
    {
        return _regs;
    }
    void Thread::registers(const Registers &regs)
    {
        _regs = regs;
    }

    void Thread::to_stream(std::ostream &os) const
    {
        os << "PC: " << program_counter() << "\n";
        registers().to_stream(os);
    }
}
