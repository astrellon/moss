#pragma once

#include <string>

namespace moss
{
    class Utils
    {
        public:
            static void trim_str(std::string &str);
            static bool is_whitespace(char c);
            static bool is_int_digit(char c);
            static bool is_float_digit(char c, bool first_char);
            static bool is_hex_digit(char c);

            static std::string replace_ext(const std::string &input, const std::string &ext);
    };
}
