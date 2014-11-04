#include "compiler.h"

#include "common.h"
#include "utils.h"

#include <stdlib.h>

namespace moss
{
    Compiler::Compiler() :
        _index(0),
        _tokens(nullptr)
    {

    }
    Compiler::~Compiler()
    {
        if (_tokens != nullptr)
        {
            delete _tokens;
        }
        finalise();
    }

    void Compiler::process_stream(const std::string &filename, std::istream &ss)
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
                /*
                std::cout << "- Token: >" << *iter << " | " << Opcode::type_name(type); 
                if (type == Opcode::REGISTER || type == Opcode::MEMORY)
                {
                    std::cout << " | " << get_register_value(*iter);
                }
                else if (type == Opcode::INT_NUMBER)
                {
                    uint32_t value = static_cast<uint32_t>(std::stol(*iter));
                    std::cout << " | " << value;
                }
                else if (type == Opcode::FLOAT_NUMBER)
                {
                    float value = static_cast<float>(std::stof(*iter));
                    std::cout << " | " << value;
                }
                std::cout << "\n";
                */
            }

            if (first_type == Opcode::COMMAND)
            {
                auto command_name = Opcode::build_command_name(line[0], types);
                auto command = Opcode::find_command(command_name); 
                
                //std::cout << "- Line command: " << command_name << "\n";
                writeU(command);

                for (auto i = 1u; i < line.size(); i++)
                {
                    switch (types[i - 1])
                    {
                        case Opcode::INT_NUMBER:
                            writeU(static_cast<uint32_t>(std::stol(line[i])));
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

    bool Compiler::finalise()
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
                auto diff = static_cast<int32_t>(find->second) - index;
                _data[*iter2].i = diff;
            }
        }

        _label_temp.clear();
        return true;
    }

    void Compiler::add_label(const std::string &label)
    {
        _label_locations[label] = _index;
    }

    void Compiler::writeU(uint32_t value)
    {
        _data.push_back(DataWord(value));
        ++_index;
    }
    void Compiler::writeI(int32_t value)
    {
        _data.push_back(DataWord(value));
        ++_index;
    }
    void Compiler::writeF(float value)
    {
        _data.push_back(DataWord(value));
        ++_index;
    }
    void Compiler::writeL(const std::string &label)
    {
        _label_temp[label].push_back(_index);
        writeU(0u);
    }

    Opcode::Type Compiler::get_token_type(const std::string &token, bool is_first_token)
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

        if (Utils::is_digit(token[0], true))
        {
            bool is_float = false;
            for (auto i = 1u; i < token.size(); ++i)
            {
                if (!Utils::is_digit(token[i], true))
                {
                    return Opcode::UNKNOWN_TYPE;
                }
                if (token[i] == '.' || token[i] == 'f')
                {
                    is_float = true;
                }
            }
            return is_float ? Opcode::FLOAT_NUMBER : Opcode::INT_NUMBER;
        }
        if (token[0] == 'r' && Utils::is_digit(token[1], false))
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

    bool Compiler::is_register(const std::string &token, std::size_t index)
    {
        if (index >= token.size() || token[index] != 'r')
        {
            return false;
        }

        for (auto i = index + 1; i < token.size(); ++i)
        {
            if (!Utils::is_digit(token[i], false))
            {
                return false;
            }
        }
        return true;
    }

    uint32_t Compiler::get_register_value(const std::string &value)
    {
        auto index = 0u;
        while (!Utils::is_digit(value[index], false))
        {
            ++index;
        }
        if (index >= value.size())
        {
            return -1;
        }

        std::string sub = value.substr(index);
        return static_cast<uint32_t>(std::stol(sub));
    }

    std::string Compiler::process_label(const std::string &token)
    {
        if (token.back() == ':')
        {
            return token.substr(0, token.size() - 1);
        }
        return token;
    }
}
