#include <iostream>

#include "moss/cpu.h"
#include "moss/cpu_arm.h"
#include "moss/memory.h"
#include "moss/memory_writer.h"
#include "moss/assembler.h"
#include "moss/disassembler.h"
#include "moss/test_peripheral.h"
#include "moss/common.h"

#include <string>
#include <sstream>
#include <fstream>
#include <exception>
#include <time.h>

int main(int argc, char **argv)
{
    moss::Memory mem(128u * 1024u);
    mem.zero();

    moss::CpuArm cpu(4u);

    moss::TestPeripheral testPerf;
    cpu.install_peripheral(&testPerf);

    uint32_t program_start = 128;
    cpu.registers().program_counter(program_start);
    cpu.memory(&mem);

    moss::Assembler assembler;
    std::ifstream input_ss("test3.asm");
    assembler.process_stream(std::string("test3.asm"), input_ss);
    assembler.finalise();
    assembler.write_to_memory<moss::Memory>(&mem, program_start);

    moss::Disassembler::to_stream<moss::Memory>(std::cout, &mem, program_start, program_start + 158);

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

#ifdef _WIN32
	std::cin.get();
#endif

    return 0;
}

