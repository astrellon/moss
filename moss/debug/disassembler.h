#pragma once

#include <iostream>
#include <stdint.h>
#include <iomanip>

#include <moss/base/opcode.h>
#include <moss/cpu/registers.h>

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
                            ss << std::setw(3) << cond_name << ' ';
                        }
                        else
                        {
                            ss << "    ";
                        }
                        auto types = Opcode::get_opcode_types(static_cast<Opcode::Command>(code));

                        ss << std::setw(8) << i << " - " << types.first;

                        if (types.second.size() > 0)
                        {
                            ss << ':';
                            for (auto iter = types.second.begin(); iter != types.second.end(); ++iter)
                            {
                                ss << ' ';
                                switch (*iter)
                                {
                                    case Opcode::INT_NUMBER:
                                        ss << mem->int_data(++i);
                                        break;
                                    case Opcode::FLOAT_NUMBER:
                                        ss << mem->float_data(++i);
                                        break;
                                    case Opcode::MEMORY:
                                        ss << '@';
                                    case Opcode::REGISTER:
                                        ss << 'r' << mem->uint_data(++i);
                                        break;
                                    case Opcode::NUMBER:
                                        ++i;
                                        ss << '[' << mem->int_data(i) << " | " << mem->float_data(i) << ']';
                                        break;
                                    case Opcode::FLAG:
                                        ss << Registers::flag_name(mem->uint_data(++i));
                                        break;
                                    case Opcode::STRING:
                                        //ss << "<insert string here>";
                                        ++i;
                                        {
                                            int32_t i2 = mem->int_data(i) + i + 1;
                                            ss << '"';
                                            while (true)
                                            {
                                                uint32_t value = mem->uint_data(i2++);
                                                char c1 = (value >> 24) & 0xFF;
                                                char c2 = (value >> 16) & 0xFF;
                                                char c3 = (value >> 8) & 0xFF;
                                                char c4 = value & 0xFF;
                                                if (c1) ss << c1; else break;
                                                if (c2) ss << c2; else break;
                                                if (c3) ss << c3; else break;
                                                if (c4) ss << c4; else break;
                                            }
                                            ss << '"';
                                        }
                                        break;
                                    case Opcode::NAMED_REGISTER:
                                        ss << Opcode::named_register_name(mem->uint_data(++i));
                                        break;
                                    default:
                                        ss << "Unknown";
                                }
                            }
                        }
                        ss << "\n";
                    }
                }
    };
}
