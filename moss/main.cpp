#include <iostream>

#include "cpu.h"
#include "memory.h"

int main(int argc, char **argv)
{
    moss::Memory mem(1024u);
    mem.zero();

    for (int i = 0; i < 32; i++)
    {
        mem.data(i, i);
    }
    uint32_t index = mem.add_command(100, moss::Cpu::MOV_I_R, 40, 0);
    index = mem.add_command(index, moss::Cpu::MOV_I_R, 55, 1);
    index = mem.add_command(index, moss::Cpu::ADD_R_R_R, 0, 1, 2);
    index = mem.add_command(index, moss::Cpu::PUSH_I, 65);
    index = mem.add_command(index, moss::Cpu::JMP_I, index + 4);
    index = mem.add_command(index, moss::Cpu::PUSH_I, 78);
    index = mem.add_command(index, moss::Cpu::POP_R, 5);
    //index = mem.add_command(index, moss::Cpu::POP_R, 6);

    mem.to_stream(std::cout, 0, 16);

    moss::Cpu cpu(4u);
    cpu.registers().program_counter(100);
    cpu.memory(&mem);
    cpu.run();
    cpu.to_stream(std::cout);

    return 0;
}
