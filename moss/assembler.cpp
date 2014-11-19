#include "assembler.h"

#include "common.h"
#include "utils.h"

#include <stdlib.h>
#include <sstream>

namespace moss
{
    Assembler::Assembler() :
        _index(0),
        _tokens(nullptr),
		_stack_pointer_index(0),
		_code_stack_pointer_index(0)
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

    Assembler::Report::Report() :
        total_size(0u),
        num_strings(0u),
        num_labels(0u)
    {

    }

    Assembler::Report Assembler::report() const
    {
        return _report;
    }

    void Assembler::process_stream(const std::string &filename, std::istream &ss)
    {
        // Setup stack pointer
        writeU(Opcode::MOV_NR_I);
        writeU(Opcode::STACK_POINTER);
		_stack_pointer_index = _index;
        writeU(0u);
        
        // Setup code stack pointer
        writeU(Opcode::MOV_NR_I);
        writeU(Opcode::CODE_STACK_POINTER);
		_code_stack_pointer_index = _index;
        writeU(0u);
        
        _tokens = new Tokeniser(ss);

        while (_tokens->has_tokens())
        {
            auto line = _tokens->next_token_line();
            std::cout << "New line\n";
            Opcode::Type first_type = Opcode::UNKNOWN_TYPE;
            Opcode::Conditionals condition = Opcode::COND_NONE;
            uint32_t type_index = 0;

            std::vector<Opcode::Type> types;
            bool first = true;
            for (auto iter = line.begin(); iter != line.end(); ++iter)
            {
                Opcode::Type type = get_token_type(*iter, first);
                std::cout << "- " << *iter << " | " << Opcode::type_name(type) << "\n";
                
                if (type == Opcode::CONDITION)
                {
                    condition = Opcode::find_conditional(*iter);
                    type_index = 1;
                }
                else if (first)
                {
                    first_type = type;
                    first = false;
                }
                else
                {
                    types.push_back(type);
                }
            }

            if (first_type == Opcode::COMMAND)
            {
                auto command_name = Opcode::build_command_name(line[type_index], types);
                uint32_t command = static_cast<uint32_t>(Opcode::find_command(command_name)); 
                if (condition != Opcode::COND_NONE)
                {
                    std::cout << "- Before cond: " << std::hex << command;
                    command |= static_cast<uint32_t>(condition);
                    std::cout << " | " << std::hex << command << std::dec << "\n";
                }
                
                writeU(command);

                for (auto i = type_index + 1; i < line.size(); i++)
                {
                    switch (types[i - type_index - 1])
                    {
                        case Opcode::INT_NUMBER:
                            writeU(parse_int(line[i]));
                            break;
                        case Opcode::FLOAT_NUMBER:
                            writeF(static_cast<float>(atof(line[i].c_str())));
                            break;
                        case Opcode::REGISTER:
                        case Opcode::MEMORY:
                            writeU(get_register_value(line[i]));
                            break;
                        case Opcode::LABEL:
                            writeL(line[i]);
                            break;
                        case Opcode::STRING:
                            writeS(line[i]);
                            break;
                        case Opcode::FLAG:
                            writeU(static_cast<uint32_t>(Opcode::find_flag(line[i])));
                            break;
                        case Opcode::NAMED_REGISTER:
                            writeU(static_cast<uint32_t>(Opcode::find_named_register(line[i])));
                            break;
                        default:
                            std::cout << "Unknown opcode type: " << types[i - type_index - 1] << "\n";
                    }
                }
            }
            else if (first_type == Opcode::LABEL)
            {
                add_label(process_label(line[0]));
            }
            else if (first_type == Opcode::VARIABLE)
            {
                if (condition != Opcode::COND_NONE)
                {
                    std::cout << "Cannot declare varible with conditional!\n";
                    return;
                }

                DataWord value;
                switch (types[0])
                {
                    default:
                        std::cout << "Invalid type for variable: " << types[0] << "\n";
                        break;
                    case Opcode::INT_NUMBER:
                        value.u = parse_int(line[1]);
                        break;
                    case Opcode::FLOAT_NUMBER:
                        value.f = static_cast<float>(atof(line[1].c_str()));
                        break;
                    case Opcode::REGISTER:
                    case Opcode::MEMORY:
                        value.u = get_register_value(line[1]);
                        break;
                    case Opcode::LABEL:
                        // TODO
                        //writeL(line[1]);
                        break;
                    case Opcode::STRING:
                        // TODO
                        //writeS(line[1]);
                        break;
                    case Opcode::FLAG:
                        value.u = static_cast<uint32_t>(Opcode::find_flag(line[1]));
                        break;
                    case Opcode::NAMED_REGISTER:
                        value.u = static_cast<uint32_t>(Opcode::find_named_register(line[1]));
                        break;
                }
                add_variable(process_variable(line[0]), types[0], value);
            }
        }
    }

    bool Assembler::finalise()
    {
        _report.num_labels += _label_temp.size();
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

        for (auto i = 0; i < 8; i++)
        {
            writeU(Opcode::HALT);
        }

        _report.num_strings += _string_temp.size();
        for (auto iter = _string_temp.begin(); iter != _string_temp.end(); ++iter)
        {
            for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2)
            {
                auto index = static_cast<int32_t>(*iter2);
                auto diff = static_cast<int32_t>(_index) - index - 1;
                _data[*iter2].i = diff;
            }

            const std::string &str = iter->first;
            auto j = 0u;
            DataWord str_value;
            for (auto i = 0u; i < str.size(); ++i)
            {
                str_value.b[3 - j] = str[i];
                ++j;
                if (j >= 4)
                {
                    _data.push_back(str_value);
                    str_value = DataWord(0u);
                    ++_index;
                    j = 0u;
                }
            }
            if (j > 0)
            {
                _data.push_back(str_value);
                ++_index;
            }
            writeU(0u);
        }

        _report.total_size = _index;

		_data[_code_stack_pointer_index].u = _index + 4;
		_data[_stack_pointer_index].u = _index + 1028;
        return true;
    }

    void Assembler::add_label(const std::string &label)
    {
        _label_locations[label] = _index;
    }
    void Assembler::add_variable(const std::string &name, Opcode::Type type, DataWord value)
    {
        _variables[name] = ValuePair { type, value };
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
    void Assembler::writeS(const std::string &str)
    {
        _string_temp[process_string_value(str)].push_back(_index);
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
        if (token.back() == '=')
        {
            return Opcode::VARIABLE;
        }

        if (is_first_token)
        {
            if (Opcode::find_conditional(token) != Opcode::COND_UNKNOWN)
            {
                return Opcode::CONDITION;
            }

            return Opcode::COMMAND;
        }

        // Is a number
        if (Utils::is_int_digit(token[0]) ||
            Utils::is_float_digit(token[0], true))
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
                    !Utils::is_float_digit(token[i], false) &&
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

        // Is register
        if (token[0] == 'r' && Utils::is_int_digit(token[1]))
        {
            if (is_register(token, 0))
            {
                return Opcode::REGISTER;
            }
        }

        // Is memory
        if (token[0] == '@')
        {
            if (is_register(token, 1))
            {
                return Opcode::MEMORY;
            }
            return Opcode::UNKNOWN_TYPE;
        }

        // Is a string.
        if (token[0] == '"' && token.back() == '"')
        {
            return Opcode::STRING;
        }

        // Is flag string
        if (Opcode::find_flag(token) != Registers::FLAG_UNKNOWN)
        {
            return Opcode::FLAG;
        }

        if (Opcode::find_named_register(token) != Opcode::NAMED_UNKNOWN)
        {
            return Opcode::NAMED_REGISTER;
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
    std::string Assembler::process_variable(const std::string &token)
    {
        if (token.back() == '=')
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
        return static_cast<uint32_t>(atoi(str.c_str()));
    }

    std::string Assembler::process_string_value(const std::string &str)
    {
        return str.substr(1, str.size() - 2);
    }
}
