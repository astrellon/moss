#include <iostream>

#include "cpu.h"
#include "memory.h"
#include "registers.h"

int main(int argc, char **argv)
{
    moss::Memory mem(1024u);
    mem.zero();
    mem.data(0, moss::Cpu::MOV_I_R);
    mem.data(1, 40);
    mem.data(2, 0);
    mem.data(3, moss::Cpu::MOV_I_R);
    mem.data(4, 55);
    mem.data(5, 1);
    mem.data(6, moss::Cpu::ADD_R_R);
    mem.data(7, 0);
    mem.data(8, 1);

    moss::Cpu cpu;
    cpu.memory(&mem);
    cpu.run();
    cpu.to_stream(std::cout);

    return 0;
}
