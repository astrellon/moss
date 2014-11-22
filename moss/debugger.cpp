#include "debugger.h"

#include <iostream>

#include "cpu_arm.h"

namespace moss
{
    Debugger::Debugger(CpuArm *cpu) :
        _cpu(cpu)
    {
         
    }

    void Debugger::on_break()
    {
        _cpu->to_stream(std::cout);
    }

}
