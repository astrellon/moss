#include <iostream>

#include "moss/cpu_arm.h"
#include "moss/memory.h"
#include "moss/memory_writer.h"
#include "moss/assembler.h"
#include "moss/disassembler.h"
#include "moss/test_peripheral.h"
#include "moss/common.h"
#include "moss/preprocessor.h"
#include "moss/debugger.h"

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

    {
        std::ifstream input_ss("test3.asm");
        std::ofstream output_ss;
        output_ss.open("preproc_temp.asm");
        moss::Preprocessor preproc(input_ss, output_ss);
        preproc.process_stream();
    }

    {
        moss::Assembler assembler;
        std::ifstream input_ss("preproc_temp.asm");
        assembler.process_stream(std::string("test3.asm"), input_ss);
        assembler.finalise();
        assembler.write_to_memory<moss::Memory>(&mem, program_start);

        auto report = assembler.report();
        moss::Disassembler::to_stream<moss::Memory>(std::cout, &mem, program_start, program_start + report.total_size);
    }

    moss::Debugger debugger(&cpu);
    cpu.remote_debugger(true);

    try
    {
		clock_t start = clock();
        int cpu_result = 0;
        do
        {
            cpu_result = cpu.run();

            if (cpu_result == 1)
            {
                debugger.on_break(); 
            }
        }
        while (cpu_result > 0);
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

