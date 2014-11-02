#include "tokeniser.h"

namespace moss
{
    Tokeniser::Tokeniser(std::istream &input) :
        _has_tokens(false),
        _end_of_stream(false),
        _current_token_index(0),
        _input(input)
    {
        process_line();
    }

    bool Tokeniser::has_tokens() const
    {
        return _has_tokens;
    }
    bool Tokeniser::end_of_stream() const
    {
        return _end_of_stream;
    }

    std::string Tokeniser::next_token()
    {
        std::string result = _current_tokens[_current_token_index++]; 
        if (_current_token_index >= _current_tokens.size())
        {
            process_line();
        }
        return result;
    }

    void Tokeniser::process_line()
    {
        _current_token_index = 0;
        _has_tokens = false;
        _current_tokens.clear();

        std::string line;
        do
        {
            if (!std::getline(_input, line))
            {
                _end_of_stream = true;
                return;
            }

            trim_str(line);
        } while (line.size() == 0);

        _has_tokens = true;

        auto start = 0u;
        auto end = next_whitespace(line, 0u);
        do
        {
            _current_tokens.push_back(line.substr(start, end - start));

            start = next_non_whitespace(line, end);
            end = next_whitespace(line, start);
        } while (start < line.size());

        _current_tokens.push_back(std::string());
    }

    void Tokeniser::trim_str(std::string &str)
    {
        auto start = 0u;
        auto end = str.size() - 1;

        while (start <= end && is_whitespace(str, start))
        {
            ++start;
        }
        while (end > 0 && is_whitespace(str, end))
        {
            --end;
        }

        str = str.substr(start, end - start + 1);
    }
    bool Tokeniser::is_whitespace(const std::string &str, std::size_t index)
    {
        auto c = str[index];
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    std::size_t Tokeniser::next_whitespace(const std::string &str, std::size_t index)
    {
        while (index < str.size() && !is_whitespace(str, index))
        {
            ++index;
        }
        return index;
    }
    std::size_t Tokeniser::next_non_whitespace(const std::string &str, std::size_t index)
    {
        while (index < str.size() && is_whitespace(str, index))
        {
            ++index;
        }
        return index;
    }
}
