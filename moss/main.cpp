#include <iostream>

#include "cpu.h"
#include "memory.h"
#include "memory_writer.h"

int main(int argc, char **argv)
{
    moss::Memory mem(1024u);
    mem.zero();

    for (int i = 0; i < 32; i++)
    {
        mem.data(i, i);
    }
    mem.data(2, 5);
    mem.data(5, 2);
    mem.data(3, 8);
    mem.data(8, 3);

    moss::Cpu cpu(4u);

    cpu.registers().program_counter(64);
    cpu.memory(&mem);

    moss::MemoryWriter<moss::Mmu> dataWriter(32, &cpu.mmu());
    for (auto i = 0; i < 32; i++)
    {
        dataWriter.write(i * 4);
    }
    
    moss::MemoryWriter<moss::Mmu> codeWriter(64, &cpu.mmu());
    codeWriter.write(moss::Cpu::MOV_I_R, 40, 0);
    codeWriter.write(moss::Cpu::MOV_I_R, 55, 1);
    codeWriter.write(moss::Cpu::ADD_R_R_R, 0, 1, 2);
    
    cpu.mmu().to_stream(std::cout, true, 0, 128);
   
    cpu.run();
    cpu.to_stream(std::cout);

    return 0;
}
