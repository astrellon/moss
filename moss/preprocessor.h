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
            Preprocessor();

            void process_stream(std::istream &ss, std::ostream &os);

        private:
            std::map<std::string, std::string> _symbols;

            void process_preprocess_line(const std::vector<std::string> &line);
            void process_normal_line(const std::vector<std::string> &line);
            void add_symbol(const std::string &key, const std::string &value);
    };
}
