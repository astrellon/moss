#pragma once

#include <iostream>
#include <stdint.h>
#include <iomanip>

#include "opcode.h"

namespace moss
{
    class mem;

    class Disassembler
    {
        public:

            template <class T>
                static void to_stream(std::ostream &ss, T *mem, uint32_t start, uint32_t end)
                {
                    for (auto i = start; i < end; ++i)
                    {
                        auto code = mem->uint_data(i);
                        if (code > Opcode::COND_ANY)
                        {
                            auto cond = code & Opcode::COND_ALL;
                            auto cond_name = Opcode::conditional_name(cond);
                            code &= ~(Opcode::COND_ALL);
                            std::cout << std::setw(3) << cond_name << ' ';
                        }
                        else
                        {
                            std::cout << "    ";
                        }
                        auto types = Opcode::get_opcode_types(static_cast<Opcode::Command>(code));

                        std::cout << std::setw(8) << i << " - " << types.first;

                        if (types.second.size() > 0)
                        {
                            std::cout << ':';
                            for (auto iter = types.second.begin(); iter != types.second.end(); ++iter)
                            {
                                std::cout << ' ';
                                switch (*iter)
                                {
                                    case Opcode::INT_NUMBER:
                                        std::cout << mem->int_data(++i);
                                        break;
                                    case Opcode::FLOAT_NUMBER:
                                        std::cout << mem->float_data(++i);
                                        break;
                                    case Opcode::MEMORY:
                                        std::cout << '@';
                                    case Opcode::REGISTER:
                                        std::cout << 'r' << mem->uint_data(++i);
                                        break;
                                    case Opcode::NUMBER:
                                        ++i;
                                        std::cout << '[' << mem->int_data(i) << " | " << mem->float_data(i) << ']';
                                        break;
                                    case Opcode::FLAG:
                                        std::cout << Opcode::flag_name(mem->uint_data(++i));
                                        break;
                                    case Opcode::STRING:
                                        //std::cout << "<insert string here>";
                                        ++i;
                                        {
                                            int32_t i2 = mem->int_data(i) + i + 1;
                                            std::cout << '"';
                                            while (true)
                                            {
                                                uint32_t value = mem->uint_data(i2++);
                                                char c1 = (value >> 24) & 0xFF;
                                                char c2 = (value >> 16) & 0xFF;
                                                char c3 = (value >> 8) & 0xFF;
                                                char c4 = value & 0xFF;
                                                if (c1) std::cout << c1; else break;
                                                if (c2) std::cout << c2; else break;
                                                if (c3) std::cout << c3; else break;
                                                if (c4) std::cout << c4; else break;
                                            }
                                            std::cout << '"';
                                        }
                                        break;
                                    case Opcode::NAMED_REGISTER:
                                        std::cout << Opcode::named_register_name(mem->uint_data(++i));
                                        break;
                                    default:
                                        std::cout << "Unknown";
                                }
                            }
                        }
                        std::cout << "\n";
                    }
                }
    };
}
