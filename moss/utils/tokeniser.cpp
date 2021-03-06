#include "tokeniser.h"

#include "utils.h"

namespace moss
{
    Tokeniser::Tokeniser(std::istream &input, bool include_all_lines) :
        _has_tokens(false),
        _end_of_stream(false),
        _in_comment(false),
        _in_string(false),
        _include_all_lines(include_all_lines),
        _current_line(0),
        _input(input)
    {
        do
        {
            process_line();
        } while (!_end_of_stream && _current_tokens.size() == 0);
    }

    bool Tokeniser::has_tokens() const
    {
        return _has_tokens;
    }
    bool Tokeniser::end_of_stream() const
    {
        return _end_of_stream;
    }

    uint32_t Tokeniser::current_line() const
    {
        return _current_line;
    }

    std::vector<std::string> Tokeniser::next_token_line()
    {
        std::vector<std::string> result = _current_tokens;
        do
        {
            process_line();
        } while (!_end_of_stream && _current_tokens.size() == 0);
        return result;
    }

    void Tokeniser::process_line()
    {
        _has_tokens = false;
        _in_comment = false;
        _in_string = false;
        _current_tokens.clear();

        std::string line;
        do
        {
            _current_line++;

            if (!std::getline(_input, line))
            {
                _end_of_stream = true;
                return;
            }

            Utils::trim_str(line);
        } while (line.size() == 0 && !_include_all_lines);

        _has_tokens = true;

        auto start = 0u;
        auto end = next_whitespace(line, 0u);
        if (_in_comment && !_include_all_lines)
        {
            return;
        }

        do
        {
            _current_tokens.push_back(line.substr(start, end - start));

            start = next_non_whitespace(line, end);
            end = next_whitespace(line, start);
        } while (!_in_comment && start < line.size());
    }

    std::size_t Tokeniser::next_whitespace(const std::string &str, std::size_t index)
    {
        while (index < str.size() && (_in_string || !Utils::is_whitespace(str[index])))
        {
            if (!_include_all_lines && str[index] == ';')
            {
                _in_comment = true;
                return index;
            }
            if (str[index] == '"')
            {
                if (index == 0 || (index > 0 && str[index - 1] != '\\'))
                {
                    _in_string = !_in_string;
                }
            }
            ++index;
        }
        return index;
    }
    std::size_t Tokeniser::next_non_whitespace(const std::string &str, std::size_t index)
    {
        while (index < str.size() && Utils::is_whitespace(str[index]))
        {
            if (str[index] == ';')
            {
                _in_comment = true;
                return index;
            }
            ++index;
        }
        return index;
    }
}
