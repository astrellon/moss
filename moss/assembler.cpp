#include "assembler.h"

#include "common.h"
#include "utils.h"

#include <stdlib.h>
#include <sstream>
#include <fstream>

namespace moss
{
    Assembler::Assembler() :
        _index(0),
        _enable_debug_symbols(false),
		_stack_pointer_index(0),
		_code_stack_pointer_index(0)
    {

    }
    Assembler::~Assembler()
    {
        auto tokens = _current_tokeniser.top();
        while (tokens)
        {
            delete tokens;
            _current_tokeniser.pop();
            if (_current_tokeniser.size() > 0)
            {
                tokens = _current_tokeniser.top();
            }
            else
            {
                break;
            }
        }
            
        finalise();
    }

    Assembler::Report::Report() :
        total_size(0u),
        num_strings(0u),
        num_labels(0u)
    {

    }

    bool Assembler::enable_debug_symbols() const
    {
        return _enable_debug_symbols;
    }
    void Assembler::enable_debug_symbols(bool enable)
    {
        _enable_debug_symbols = enable;
    }

    Assembler::Report Assembler::report() const
    {
        return _report;
    }

    void Assembler::write_setup_code()
    {
        writeU(Opcode::MOV_R_NR);
        writeU(2u);
        writeU(Opcode::PROGRAM_COUNTER);

        writeU(Opcode::MOV_R_I);
        writeU(0u);
        _stack_pointer_index = _index;
        writeU(0u);
        
        writeU(Opcode::MOV_R_I);
        writeU(1u);
        _code_stack_pointer_index = _index;
        writeU(0u);

        writeU(Opcode::ADD_R_R);
        writeU(0u);
        writeU(2u);
        
        writeU(Opcode::ADD_R_R);
        writeU(1u);
        writeU(2u);

        writeU(Opcode::MOV_NR_R);
        writeU(Opcode::STACK_POINTER);
        writeU(0u);

        writeU(Opcode::MOV_NR_R);
        writeU(Opcode::CODE_STACK_POINTER);
        writeU(1u);

        writeU(Opcode::JMP_I);
        writeL("main");
    }

    void Assembler::process_stream(const std::string &filename, std::istream &ss)
    {
        write_setup_code();

        process_include(filename, ss);
    }

    void Assembler::process_include(const std::string &filename, std::istream &ss)
    {
        _current_filename.push(filename);
        auto tokens = new Tokeniser(ss, false);
        _current_tokeniser.push(tokens);

        while (tokens->has_tokens())
        {
            auto line = tokens->next_token_line();
            std::cout << "New line @ " << tokens->current_line() << "\n";
            if (_enable_debug_symbols)
            {
                _debug_data.data(_index, tokens->current_line(), filename);
            }
            
            if (line[0][0] == '#')
            {
                // Process line as a preprocessor line (#define, #include, etc)
                process_preprocessor_line(line);
            }
            else
            {
                // Look for defined symbols in a normal line and replace.
                preprocess_normal_line(line);
                // Assemble normal line
                process_normal_line(line);
            }
        }
        _current_filename.pop();
    }

    void Assembler::preprocess_normal_line(std::vector<std::string> &line)
    {
        for (auto i = 0u; i < line.size(); i++)
        {
            auto token = line[i];
            bool is_memory = false;
            if (token[0] == '@')
            {
                is_memory = true;
                token = token.substr(1);
            }

            auto find = _symbols.find(token);
            if (find != _symbols.end())
            {
                auto result = find->second;
                if (is_memory)
                {
                    result = std::string("@") + result;
                }
                line[i] = result;
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

        if (_enable_debug_symbols)
        {
            std::ofstream debug_out;
            debug_out.open("debug.out");
            _debug_data.to_stream(debug_out);
        }
        return true;
    }
    
    void Assembler::process_preprocessor_line(std::vector<std::string> &line)
    {
        if (line[0] == "#define")
        {
            if (line.size() < 2)
            {
                add_symbol(line[1], std::string(""));
            }
            else
            {
                add_symbol(line[1], line[2]);
            }
        }
        else if (line[0] == "#include")
        {
            std::ifstream new_ss;
            std::string filename = process_string_value(line[1]);
            new_ss.open(filename);
            process_include(filename, new_ss);
        }
    }

    void Assembler::process_normal_line(std::vector<std::string> &line)
    {
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
    }

    void Assembler::add_symbol(const std::string &key, const std::string &value)
    {
        auto find = _symbols.find(key);
        if (find != _symbols.end())
        {
            std::cout << "Redefining symbol " << key << ": from " << find->second << ", to " << value << "\n";
        }
        _symbols[key] = value;
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
        
        if (is_first_token)
        {
            if (Opcode::find_conditional(token) != Opcode::COND_UNKNOWN)
            {
                return Opcode::CONDITION;
            }

            if (token.back() == ':')
            {
                return Opcode::LABEL;
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
