#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace moss
{
    class Preprocessor
    {
        public:
            Preprocessor(std::istream &ss, std::ostream &os);

            void process_stream();

        private:
            std::map<std::string, std::string> _symbols;
            std::istream &_ss;
            std::ostream &_os;

            void process_preprocessor_line(const std::vector<std::string> &line);
            void process_normal_line(const std::vector<std::string> &line);
            void add_symbol(const std::string &key, const std::string &value);

            void process_stream(std::istream &ss);

            static std::string process_string_value(const std::string &str);
    };
}
