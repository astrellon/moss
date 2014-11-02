#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stdint.h>

namespace moss
{
    class Tokeniser
    {
        public:
            Tokeniser(std::istream &input);

            bool has_tokens() const;
            bool end_of_stream() const;
            std::string next_token();
            
            static void trim_str(std::string &str);
            static bool is_whitespace(const std::string &str, std::size_t index);

        private:
            bool _has_tokens;
            bool _end_of_stream;
            uint32_t _current_token_index;

            std::istream &_input;
            std::vector<std::string> _current_tokens;

            void process_line();

            static std::size_t next_whitespace(const std::string &str, std::size_t index);
            static std::size_t next_non_whitespace(const std::string &str, std::size_t index);
    };
}
