#include <iostream>

#include <moss/cpu/cpu_arm.h>
#include <moss/cpu/memory.h>
#include <moss/cpu/memory_writer.h>
#include <moss/assemble/assembler.h>
#include <moss/debug/disassembler.h>
#include <moss/cpu/test_peripheral.h>
#include <moss/utils/common.h>
#include <moss/utils/utils.h>
#include <moss/debug/debugger.h>

#include <string>
#include <sstream>
#include <fstream>
#include <exception>
#include <time.h>

int main(int argc, char **argv)
{
    std::string filename;
    std::string debug_symbols;
    std::string debug_config;
    std::string output_file;
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg[0] == '-')
        {

        }
        else
        {
            filename = arg;
            debug_symbols = moss::Utils::replace_ext(arg, "symbols");
            debug_config = moss::Utils::replace_ext(arg, "conf");
            output_file = moss::Utils::replace_ext(arg, "bin");
        }
    }

    if (filename.empty())
    {
        std::cout << "No filename given.\n";
#ifdef _WIN32
        std::cin.get();
#endif

        return -1;
    }

    moss::Memory mem(128u * 1024u);
    mem.zero();

    moss::CpuArm cpu(4u);

    moss::TestPeripheral testPerf;
    cpu.install_peripheral(&testPerf);

    uint32_t program_start = 128;
    cpu.registers().program_counter(program_start);
    cpu.memory(&mem);

    {
        // Setup assembler
        moss::Assembler assembler;
        assembler.enable_debug_symbols(true);
        assembler.debug_symbol_filename(debug_symbols);

        // Read main input file
        std::ifstream input_ss(filename);
        assembler.process_stream(filename, input_ss);
        assembler.finalise();
        assembler.write_to_memory<moss::Memory>(&mem, program_start);
        std::ofstream output_binary(output_file);
        //assembler.write_to_stream(output_binary);

        auto report = assembler.report();
        moss::Disassembler::to_stream<moss::Memory>(std::cout, &mem, program_start, program_start + report.total_size);
    }

    moss::Debugger debugger(&cpu, program_start);
    {
        // Load debug symbols into debugger.
        std::ifstream input_debug(debug_symbols);
        debugger.load_debug_data(input_debug);
    }
    
    {
        // Load debug config data.
        std::ifstream input_debug_config(debug_config);
        if (input_debug_config)
        {
            debugger.load_debug_config(input_debug_config);
        }
    }

    cpu.remote_debugger(true);
    auto total_time = 0.0f;

    std::cout << "--- Starting CPU run ---\n";
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
		total_time = (float)(end - start) / CLOCKS_PER_SEC;
    }
    catch (std::exception e)
    {
        std::cout << "Error running CPU!\n";
        std::cout << "- Message: " << e.what() << "\n";
        std::cout << "- Stopped at: " << (cpu.registers().program_counter() - 1) << "\n";
    }

    std::cout << "\n--- CPU Info at halt ---\n";
    std::cout << "Time taken: " << total_time << "\n";
    cpu.to_stream(std::cout);

    {
        std::ofstream output_debug_config(debug_config); 
        debugger.save_debug_config(output_debug_config);
    }

#ifdef _WIN32
	std::cin.get();
#endif

    return 0;
}

