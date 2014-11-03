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
            std::cout << "New line\n";
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
                std::cout << "- Token: >" << *iter << " | " << Opcode::type_name(type) << "\n";
            }

            if (first_type == Opcode::COMMAND)
            {
                std::string command_name = Opcode::build_command_name(line[0], types);
                std::cout << "- Line command: " << command_name << "\n";
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
                _data[*iter2].u = find->second;
            }
        }

        _label_temp.clear();
        return true;
    }

    void Compiler::add_label(const std::string &label)
    {
        _label_locations[std::string(label)] = _index;
    }

    void Compiler::write(uint32_t v1)
    {
        _data.push_back(DataWord(v1));
        ++_index;
    }
    void Compiler::write(uint32_t v1, uint32_t v2)
    {
        _data.push_back(DataWord(v1));
        _data.push_back(DataWord(v2));
        _index += 2;
    }
    void Compiler::write(uint32_t v1, uint32_t v2, uint32_t v3)
    {
        _data.push_back(DataWord(v1));
        _data.push_back(DataWord(v2));
        _data.push_back(DataWord(v3));
        _index += 3;
    }
    void Compiler::write(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4)
    {
        _data.push_back(DataWord(v1));
        _data.push_back(DataWord(v2));
        _data.push_back(DataWord(v3));
        _data.push_back(DataWord(v4));
        _index += 4;
    }

    void Compiler::writeF(uint32_t v1, float v2)
    {
        _data.push_back(DataWord(v1));
        _data.push_back(DataWord(v2));
        _index += 2;
    }
    void Compiler::writeF(uint32_t v1, float v2, float v3)
    {
        _data.push_back(DataWord(v1));
        _data.push_back(DataWord(v2));
        _data.push_back(DataWord(v3));
        _index += 3;
    }
    void Compiler::writeF(uint32_t v1, float v2, float v3, float v4)
    {
        _data.push_back(DataWord(v1));
        _data.push_back(DataWord(v2));
        _data.push_back(DataWord(v3));
        _data.push_back(DataWord(v4));
        _index += 4;
    }

    void Compiler::write(uint32_t v1, const char *label)
    {
        _data.push_back(DataWord(v1));
        _label_temp[std::string(label)].push_back(_index + 1);
        _data.push_back(DataWord(0u));
        _index += 2;
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
        if (!is_register(value, 0))
        {
            return -1;
        }

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
}
