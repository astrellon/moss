#include <iostream>

#include "moss/cpu.h"
#include "moss/cpu_arm.h"
#include "moss/memory.h"
#include "moss/memory_writer.h"
#include "moss/assembler.h"
#include "moss/disassembler.h"
#include "moss/test_peripheral.h"

#include <string>
#include <sstream>
#include <fstream>
#include <exception>
#include <time.h>

int main(int argc, char **argv)
{
    moss::Memory mem(1024u);
    mem.zero();

    for (int i = 0; i < 32; i++)
    {
        mem.uint_data(i, i);
    }

    moss::CpuArm cpu(4u);

    moss::TestPeripheral testPerf;
    cpu.install_peripheral(&testPerf);

    cpu.registers().program_counter(64);
    cpu.memory(&mem);

    moss::MemoryWriter<moss::Mmu> dataWriter(32, &cpu.mmu());
    for (auto i = 0; i < 32; i++)
    {
        dataWriter.write(i * 4);
    }

    moss::Assembler assembler;
    std::ifstream input_ss("test3.asm");
    assembler.process_stream(std::string("test3.asm"), input_ss);
    assembler.finalise();
    assembler.write_to_memory<moss::Mmu>(&cpu.mmu(), 64);

    moss::Disassembler::to_stream(std::cout, &cpu.mmu(), 64, 84);

    try
    {
		clock_t start = clock();
        cpu.run();
		clock_t end = clock();
		float time = (float)(end - start) / CLOCKS_PER_SEC;
		std::cout << "Time taken: " << time << "\n";
    }
    catch (std::exception e)
    {
        std::cout << "Error running CPU!\n";
        std::cout << "- Message: " << e.what() << "\n";
        std::cout << "- Stopped at: " << (cpu.registers().program_counter() - 1) << "\n";
    }
    cpu.to_stream(std::cout);

	std::cin.get();

    return 0;
}

