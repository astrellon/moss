#pragma once

#include <string>

namespace moss
{
    class Utils
    {
        public:
            static void trim_str(std::string &str);
            static bool is_whitespace(char c);
            static bool is_digit(char c, bool include_float);
    };
}
