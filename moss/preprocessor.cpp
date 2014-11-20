#include "preprocessor.h"

#include "tokeniser.h"

#include <fstream>

namespace moss
{
    Preprocessor::Preprocessor(std::istream &ss, std::ostream &os) :
        _ss(ss),
        _os(os)
    {
    }

    void Preprocessor::process_stream()
    {
        process_stream(_ss);
    }
    void Preprocessor::process_stream(std::istream &ss)
    {
        Tokeniser tokens(ss);

        while (tokens.has_tokens())
        {
            auto line = tokens.next_token_line();

            if (line[0][0] == '#')
            {
                process_preprocessor_line(line);
            }
            else
            {
                process_normal_line(line);
            }
        }
    }

    void Preprocessor::process_preprocessor_line(const std::vector<std::string> &line)
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
            new_ss.open(process_string_value(line[1]));
            process_stream(new_ss);
        }
    }

    void Preprocessor::process_normal_line(const std::vector<std::string> &line)
    {
        bool first = true;
        for (auto iter = line.begin(); iter != line.end(); ++iter)
        {
            bool isMemoryReference = false;
            auto token = *iter;
            if (token[0] == '@')
            {
                isMemoryReference = true;
                token = token.substr(1);
            }
            auto find = _symbols.find(token);
            if (find != _symbols.end())
            {
                token = find->second;
            }

            if (isMemoryReference)
            {
                token = std::string("@") + token;
            }
            
            if (first && token.back() != ':')
            {
                _os << '\t';
            }
            first = false;
            _os << token << ' ';
        }
        _os << '\n';
    }

    void Preprocessor::add_symbol(const std::string &key, const std::string &value)
    {
        auto find = _symbols.find(key);
        if (find != _symbols.end())
        {
            std::cout << "Redefining symbol " << key << ": from " << find->second << ", to " << value << "\n";
        }
        _symbols[key] = value;
    }
    
    std::string Preprocessor::process_string_value(const std::string &str)
    {
        return str.substr(1, str.size() - 2);
    }
}
