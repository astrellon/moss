#include <iostream>

#include "moss/cpu.h"
#include "moss/memory.h"
#include "moss/memory_writer.h"

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
    codeWriter.writeF(moss::Cpu::MOVF_R_I, 0, 34.5f);
    codeWriter.write(moss::Cpu::MOV_R_I, 0, 0);
    codeWriter.write(moss::Cpu::MOV_R_I, 1, 32u);
    codeWriter.add_label("start"); 
    codeWriter.write(moss::Cpu::CMP_R_I, 0, 32);
    codeWriter.write(moss::Cpu::JGE_I, "end");
    codeWriter.write(moss::Cpu::MOV_R_M, 2, 1);
    codeWriter.write(moss::Cpu::ADD_R_I, 0, 1);
    codeWriter.write(moss::Cpu::ADD_R_I, 1, 1);
    codeWriter.write(moss::Cpu::PRINT_R, 2u);
    codeWriter.write(moss::Cpu::JMP_I, "start");
    codeWriter.add_label("end");
    codeWriter.write(moss::Cpu::UINT_FLOAT_R_R, 1, 2);
    codeWriter.write(moss::Cpu::ADDF_R_R_R, 2, 1, 0);
    codeWriter.write(moss::Cpu::FLOAT_UINT_R_R, 5, 2);
    codeWriter.write(moss::Cpu::MOV_R_I, 6, 1337);
    codeWriter.finalise();

    cpu.mmu().to_stream(std::cout, true, 0, 128);

    cpu.run();
    cpu.to_stream(std::cout);

    return 0;
}
