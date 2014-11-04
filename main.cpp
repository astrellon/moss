#include <iostream>

#include "moss/cpu.h"
#include "moss/memory.h"
#include "moss/memory_writer.h"
#include "moss/tokeniser.h"
#include "moss/registers.h"
#include "moss/common.h"
#include "moss/compiler.h"
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

    /*
    moss::MemoryWriter<moss::Mmu> codeWriter(64, &cpu.mmu());
    codeWriter.write(moss::Opcode::MOV_R_I, 8);
    codeWriter.writeF(34.5f);
    codeWriter.write(moss::Opcode::MOV_R_I, 0, 0);
    codeWriter.write(moss::Opcode::MOV_R_I, 1, 32u);
    codeWriter.add_label("start"); 
    codeWriter.write(moss::Opcode::CMP_R_I, 0, 32);
    codeWriter.write(moss::Opcode::JGE_I, "end");
    codeWriter.write(moss::Opcode::MOV_R_M, 2, 1);
    codeWriter.write(moss::Opcode::ADD_R_I, 0, 1);
    codeWriter.write(moss::Opcode::ADD_R_I, 1, 1);
    codeWriter.write(moss::Opcode::PRINT_R, 2u);
    codeWriter.write(moss::Opcode::JMP_I, "start");
    codeWriter.add_label("end");
    codeWriter.write(moss::Opcode::UINT_FLOAT_R_R, 9, 2);
    codeWriter.write(moss::Opcode::ADDF_R_R_R, 10, 8, 9);
    codeWriter.write(moss::Opcode::FLOAT_UINT_R_R, 5, 10);
    codeWriter.write(moss::Opcode::MOV_R_I, 6, 1337);
    codeWriter.finalise();
    */

    moss::Compiler compiler;
    std::ifstream input_ss("test.asm");
    compiler.process_stream(std::string("test.asm"), input_ss);
    compiler.finalise();
    compiler.write_to_memory<moss::Mmu>(&cpu.mmu(), 64);

    moss::Disassembler::to_stream(std::cout, &cpu.mmu(), 64, 128);
    //cpu.mmu().to_stream(std::cout, true, 0, 128);

    try
    {
        cpu.run();
    }
    catch (std::exception e)
    {
        std::cout << "Error running CPU!\n";
    }
    cpu.to_stream(std::cout);


    /*
    std::ifstream input_ss("test.asm");
    moss::Tokeniser tokens(input_ss);
    while (tokens.has_tokens())
    {
        std::cout << "New line\n";
        std::vector<std::string> token = tokens.next_token_line();
        for (auto i = token.begin(); i != token.end(); ++i)
        {
            std::cout << "- " << *i << "\n";
        }
    }
    */

    return 0;
}
