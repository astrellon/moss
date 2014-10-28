#include <iostream>

#include "cpu.h"
#include "memory.h"
#include "registers.h"

int main(int argc, char **argv)
{
    moss::Memory mem(1024u);
    mem.zero();
    uint32_t index = mem.add_command(0, moss::Cpu::MOV_I_R, 40, 0);
    index = mem.add_command(index, moss::Cpu::MOV_I_R, 55, 1);
    index = mem.add_command(index, moss::Cpu::ADD_R_R_R, 0, 1, 2);
    index = mem.add_command(index, moss::Cpu::PUSH_R, 2);
    index = mem.add_command(index, moss::Cpu::PUSH_I, 78);
    index = mem.add_command(index, moss::Cpu::POP_R, 5);
    index = mem.add_command(index, moss::Cpu::POP_R, 6);

    mem.to_stream(std::cout, 0, 16);

    moss::Cpu cpu;
    cpu.memory(&mem);
    cpu.run();
    cpu.to_stream(std::cout);

    return 0;
}
