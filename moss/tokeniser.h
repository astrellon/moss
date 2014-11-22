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
            Tokeniser(std::istream &input, bool include_all_lines);

            bool has_tokens() const;
            bool end_of_stream() const;

            uint32_t current_line() const;

            bool include_all_lines() const;
            
            std::vector<std::string> next_token_line();

        private:
            bool _has_tokens;
            bool _end_of_stream;
            bool _in_comment;
            bool _in_string;
            bool _include_all_lines;

            uint32_t _current_line;

            std::istream &_input;
            std::vector<std::string> _current_tokens;

            void process_line();

            std::size_t next_whitespace(const std::string &str, std::size_t index);
            std::size_t next_non_whitespace(const std::string &str, std::size_t index);
    };
}
