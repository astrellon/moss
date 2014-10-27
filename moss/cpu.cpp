#include "cpu.h"

#include <iostream>

#include "memory.h"

namespace moss
{
    Cpu::Cpu() :
        _memory(nullptr),
        _running(false)
    {

    }
    Cpu::~Cpu()
    {
        stop();
    }

    Thread Cpu::thread() const
    {
        return _thread;
    }
    void Cpu::thread(const Thread &thread)
    {
        _thread = thread;
    }

    Memory *Cpu::memory() const
    {
        return _memory;
    }
    void Cpu::memory(Memory *memory)
    {
        _memory = memory;
    }

    void Cpu::run()
    {
        _running = true;
        do_run();
    }
    void Cpu::stop()
    {
        _running = false;
    }
    bool Cpu::is_running() const
    {
        return _running;
    }

    void Cpu::to_stream(std::ostream &os) const
    {
        os << "Cpu running: " << is_running() << "\n";
        _thread.to_stream(os);
    }

    uint32_t Cpu::next_int()
    {
        return _memory->data(_thread.program_counter_inc());
    }

    void Cpu::do_run()
    {
        while (_running)
        {
            uint32_t opcode = next_int();
            uint32_t ar1 = 0u;
            uint32_t ar2 = 0u;
            Registers &regs = _thread.registers();
            switch (opcode)
            {
                default:
                    std::cout << "Unknown OPCODE: " << opcode << ", halting." << std::endl;
                case HALT:
                    stop();
                    break;
                case MOV_R_R:
                    ar1 = next_int();
                    ar2 = next_int();
                    regs.int_reg(ar2, regs.int_reg(ar1));
                    break;
                case MOV_I_R:
                    ar1 = next_int();
                    ar2 = next_int();
                    regs.int_reg(ar2, ar1);
                    break;
                case MOV_M_R:
                    ar1 = next_int();
                    ar2 = next_int();
                    regs.int_reg(ar2, _memory->data(regs.int_reg(ar1)));
                    break;
                case MOV_R_M:
                    ar1 = next_int();
                    ar2 = next_int();
                    _memory->data(regs.int_reg(ar2), regs.int_reg(ar1));
                    break;
                case MOV_I_M:
                    ar1 = next_int();
                    ar2 = next_int();
                    _memory->data(ar2, ar1);
                    break;
                case MOV_M_M:
                    ar1 = next_int();
                    ar2 = next_int();
                    _memory->data(ar2, _memory->data(ar1));
                    break;
                case ADD_R_R:
                    ar1 = next_int();
                    ar2 = next_int();
                    regs.int_reg(ar2, regs.int_reg(ar1) + regs.int_reg(ar2));
                    break;


            }
        }
    }
}
