#include <iostream>

#include "moss/cpu.h"
#include "moss/memory.h"
#include "moss/memory_writer.h"
#include "moss/tokeniser.h"
#include "moss/registers.h"
#include "moss/common.h"
#include "moss/assembler.h"
#include "moss/opcode.h"
#include "moss/disassembler.h"

#include <string>
#include <sstream>
#include <fstream>
#include <exception>

int main(int argc, char **argv)
{
    moss::Memory mem(1024u);
    mem.zero();

    for (int i = 0; i < 32; i++)
    {
        mem.uint_data(i, i);
    }
    mem.uint_data(2, 5);
    mem.uint_data(5, 2);
    mem.uint_data(3, 8);
    mem.uint_data(8, 3);

    moss::Cpu cpu(4u);

    cpu.registers().program_counter(64);
    cpu.memory(&mem);

    moss::MemoryWriter<moss::Mmu> dataWriter(32, &cpu.mmu());
    for (auto i = 0; i < 32; i++)
    {
        dataWriter.write(i * 4);
    }

    moss::Assembler assembler;
    std::ifstream input_ss("test.asm");
    assembler.process_stream(std::string("test.asm"), input_ss);
    assembler.finalise();
    assembler.write_to_memory<moss::Mmu>(&cpu.mmu(), 64);

    moss::Disassembler::to_stream(std::cout, &cpu.mmu(), 64, 110);
    //cpu.mmu().to_stream(std::cout, true, 0, 128);

    try
    {
        cpu.run();
    }
    catch (std::exception e)
    {
        std::cout << "Error running CPU!\n";
        std::cout << "- Message: " << e.what() << "\n";
        std::cout << "- Stopped at: " << (cpu.registers().program_counter() - 1) << "\n";
    }
    cpu.to_stream(std::cout);

    std::cout << "Press enter to continue... ";
	std::cin.get();

    return 0;
}
