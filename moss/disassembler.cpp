#include "disassembler.h"

#include "opcode.h"
#include "mmu.h"

#include <iomanip>

namespace moss
{
    void Disassembler::to_stream(std::ostream &ss, Mmu *mmu, uint32_t start, uint32_t end)
    {
        for (auto i = start; i < end; ++i)
        {
            auto code = mmu->uint_data(i);
            auto types = Opcode::get_opcode_types(static_cast<Opcode::Command>(code));

            std::cout << std::setw(8) << i << " - " << types.first;

            if (types.second.size() > 0)
            {
                std::cout << ":";
                for (auto iter = types.second.begin(); iter != types.second.end(); ++iter)
                {
                    std::cout << " ";
                    switch (*iter)
                    {
                        case Opcode::INT_NUMBER:
                            std::cout << mmu->int_data(++i);
                            break;
                        case Opcode::FLOAT_NUMBER:
                            std::cout << mmu->float_data(++i);
                            break;
                        case Opcode::MEMORY:
                            std::cout << "@";
                        case Opcode::REGISTER:
                            std::cout << "r" << mmu->uint_data(++i);
                            break;
                        case Opcode::NUMBER:
                            ++i;
                            std::cout << mmu->int_data(i) << " | " << mmu->float_data(i);
                            break;
                        default:
                            std::cout << "Unknown";
                    }
                }
            }
            std::cout << "\n";
        }
    }
}
