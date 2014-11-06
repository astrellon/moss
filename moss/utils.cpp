#include "utils.h"

namespace moss
{
    void Utils::trim_str(std::string &str)
    {
        int start = 0;
        int end = static_cast<int>(str.size()) - 1;

        while (start <= end && is_whitespace(str[start]))
        {
            ++start;
        }
        while (end > 0 && is_whitespace(str[end]))
        {
            --end;
        }

        str = str.substr(start, end - start + 1);
    }
    
    bool Utils::is_whitespace(char c)
    {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }
    bool Utils::is_int_digit(char c)
    {
        return c >= '0' && c <= '9';
    }
    bool Utils::is_float_digit(char c)
    {
        return c == '.' || c == 'f' || is_int_digit(c);
    }
    bool Utils::is_hex_digit(char c)
    {
        return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || is_int_digit(c);
    }
}
