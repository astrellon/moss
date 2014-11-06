#include "assembler.h"

#include "common.h"
#include "utils.h"

#include <stdlib.h>
#include <sstream>

namespace moss
{
    Assembler::Assembler() :
        _index(0),
        _tokens(nullptr)
    {

    }
    Assembler::~Assembler()
    {
        if (_tokens != nullptr)
        {
            delete _tokens;
        }
        finalise();
    }

    void Assembler::process_stream(const std::string &filename, std::istream &ss)
    {
        _tokens = new Tokeniser(ss);

        while (_tokens->has_tokens())
        {
            auto line = _tokens->next_token_line();
            //std::cout << "New line\n";
            Opcode::Type first_type = Opcode::UNKNOWN_TYPE;
            std::vector<Opcode::Type> types;
            for (auto iter = line.begin(); iter != line.end(); ++iter)
            {
                bool first = iter == line.begin();
                Opcode::Type type = get_token_type(*iter, first);
                if (first)
                {
                    first_type = type;
                }
                else
                {
                    types.push_back(type);
                }
            }

            if (first_type == Opcode::COMMAND)
            {
                auto command_name = Opcode::build_command_name(line[0], types);
                auto command = Opcode::find_command(command_name); 
                
                writeU(command);

                for (auto i = 1u; i < line.size(); i++)
                {
                    switch (types[i - 1])
                    {
                        case Opcode::INT_NUMBER:
                            writeU(parse_int(line[i]));
                            break;
                        case Opcode::FLOAT_NUMBER:
                            writeF(static_cast<float>(std::stof(line[i])));
                            break;
                        case Opcode::REGISTER:
                        case Opcode::MEMORY:
                            writeU(get_register_value(line[i]));
                            break;
                        case Opcode::LABEL:
                            writeL(line[i]);
                            break;
                        default:
                            std::cout << "Unknown opcode type: " << types[i - 1] << "\n";
                    }
                }
            }
            else if (first_type == Opcode::LABEL)
            {
                add_label(process_label(line[0]));
            }
        }
    }

    bool Assembler::finalise()
    {
        for (auto iter = _label_temp.begin(); iter != _label_temp.end(); ++iter)
        {
            auto find = _label_locations.find(iter->first);
            if (find == _label_locations.end())
            {
                std::cout << "Error finalising! No label defined for: " << iter->first << std::endl;
                return false;
            }

            for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2)
            {
                auto index = static_cast<int32_t>(*iter2);
                auto diff = static_cast<int32_t>(find->second) - index - 1;
                _data[*iter2].i = diff;
            }
        }

        _label_temp.clear();
        return true;
    }

    void Assembler::add_label(const std::string &label)
    {
        _label_locations[label] = _index;
    }

    void Assembler::writeU(uint32_t value)
    {
        _data.push_back(DataWord(value));
        ++_index;
    }
    void Assembler::writeI(int32_t value)
    {
        _data.push_back(DataWord(value));
        ++_index;
    }
    void Assembler::writeF(float value)
    {
        _data.push_back(DataWord(value));
        ++_index;
    }
    void Assembler::writeL(const std::string &label)
    {
        _label_temp[label].push_back(_index);
        writeU(0u);
    }

    Opcode::Type Assembler::get_token_type(const std::string &token, bool is_first_token)
    {
        if (token.size() == 0u)
        {
            return Opcode::UNKNOWN_TYPE;
        }
        if (token.back() == ':')
        {
            return Opcode::LABEL;
        }

        if (is_first_token)
        {
            return Opcode::COMMAND;
        }

        if (Utils::is_int_digit(token[0]) ||
            Utils::is_float_digit(token[0]))
        {
            bool is_float = false;
            bool is_hex = false;
            for (auto i = 1u; i < token.size(); ++i)
            {
                if (i == 1u && token[1u] == 'x')
                {
                    if (token[0] == '0')
                    {
                        is_hex = true;
                        continue;
                    }
                    return Opcode::UNKNOWN_TYPE;
                }
                if (!Utils::is_int_digit(token[i]) &&
                    !Utils::is_float_digit(token[i]) &&
                    !Utils::is_hex_digit(token[i]))
                {
                    return Opcode::UNKNOWN_TYPE;
                }
                if (token[i] == '.' || token[i] == 'f')
                {
                    is_float = true;
                }
            }
            
            if (is_float && is_hex)
            {
                return Opcode::UNKNOWN_TYPE;
            }

            return is_float ? Opcode::FLOAT_NUMBER : Opcode::INT_NUMBER;
        }
        if (token[0] == 'r' && Utils::is_int_digit(token[1]))
        {
            if (is_register(token, 0))
            {
                return Opcode::REGISTER;
            }
        }
        if (token[0] == '@')
        {
            if (is_register(token, 1))
            {
                return Opcode::MEMORY;
            }
            return Opcode::UNKNOWN_TYPE;
        }
        return Opcode::LABEL;
    }

    bool Assembler::is_register(const std::string &token, std::size_t index)
    {
        if (index >= token.size() || token[index] != 'r')
        {
            return false;
        }

        for (auto i = index + 1; i < token.size(); ++i)
        {
            if (!Utils::is_int_digit(token[i]))
            {
                return false;
            }
        }
        return true;
    }

    uint32_t Assembler::get_register_value(const std::string &value)
    {
        auto index = 0u;
        while (!Utils::is_int_digit(value[index]))
        {
            ++index;
        }
        if (index >= value.size())
        {
            return -1;
        }

        return parse_int(value.substr(index));
    }

    std::string Assembler::process_label(const std::string &token)
    {
        if (token.back() == ':')
        {
            return token.substr(0, token.size() - 1);
        }
        return token;
    }

    uint32_t Assembler::parse_int(const std::string &str)
    {
        if (str.size() >= 3)
        {
            if (str[0] == '0' && str[1] == 'x' && Utils::is_hex_digit(str[2]))
            {
                uint32_t result;
                std::stringstream ss;
                ss << std::hex << str;
                ss >> result;
                return result;
            }
        }
        return static_cast<uint32_t>(std::stol(str));
    }
}
