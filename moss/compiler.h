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

            template <class T>
            void write_to_memory(T *memory, uint32_t offset)
            {
                for (auto i = 0u; i < _data.size(); ++i)
                {
                    memory->uint_data(i + offset, _data[i].u);
                }
            }

        private:
            uint32_t _index;
            Tokeniser *_tokens;
            std::vector<DataWord> _data;
            std::map< std::string, uint32_t > _label_locations;
            std::map< std::string, std::vector<uint32_t> > _label_temp;

            void add_label(const std::string &label);
            void writeU(uint32_t value);
            void writeI(int32_t value);
            void writeF(float value);
            void writeL(const std::string &label);

            static Opcode::Type get_token_type(const std::string &token, bool is_first_token);
            static bool is_register(const std::string &token, std::size_t index);
            static uint32_t get_register_value(const std::string &value);
            static std::string process_label(const std::string &token);

    };
}
