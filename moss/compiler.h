#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stdint.h>

#include "tokeniser.h"
#include "cpu.h"
#include "common.h"
#include "opcode.h"

namespace moss
{
    class Compiler
    {
        public:
            Compiler();
            ~Compiler();

            void process_stream(const std::string &filename, std::istream &ss);
            bool finalise();

        private:
            uint32_t _index;
            Tokeniser *_tokens;
            std::vector<DataWord> _data;
            std::map< std::string, uint32_t > _label_locations;
            std::map< std::string, std::vector<uint32_t> > _label_temp;

            void add_label(const std::string &label);
            void write(uint32_t v1);
            void write(uint32_t v1, uint32_t v2);
            void write(uint32_t v1, uint32_t v2, uint32_t v3);
            void write(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4);
            void writeF(uint32_t v1, float v2);
            void writeF(uint32_t v1, float v2, float v3);
            void writeF(uint32_t v1, float v2, float v3, float v4);
            void write(uint32_t v1, const char *label);

            static Opcode::Type get_token_type(const std::string &token, bool is_first_token);
            static bool is_register(const std::string &token, std::size_t index);
            static uint32_t get_register_value(const std::string &value);

    };
}
