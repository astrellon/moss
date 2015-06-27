#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <stdint.h>

#include <moss/utils/tokeniser.h>
#include <moss/utils/common.h>
#include <moss/base/opcode.h>
#include <moss/debug_common/debug_data.h>

namespace moss
{
    class Assembler
    {
        public:
            Assembler();
            ~Assembler();

            class Report
            {
                public:
                    Report();

                    uint32_t total_size;
                    uint32_t num_strings;
                    uint32_t num_labels;
            };

            bool enable_debug_symbols() const;
            void enable_debug_symbols(bool enable);

            const std::string &debug_symbol_filename() const;
            void debug_symbol_filename(const std::string &filename);

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
            void write_to_stream(std::ostream &ss);

            Report report() const;

            static uint32_t parse_int(const std::string &str);

        private:
            uint32_t _index;
            std::vector<DataWord> _data;
            std::map< std::string, uint32_t > _label_locations;
            std::map< std::string, std::vector<uint32_t> > _label_temp;
            std::map< std::string, std::vector<uint32_t> > _string_temp;
            std::map<std::string, std::string> _symbols;

            bool _enable_debug_symbols;
            std::string _debug_symbol_filename;
            Report _report;
            DebugData _debug_data;
            
            std::stack<Tokeniser *> _current_tokeniser;
            std::stack<std::string> _current_filename;

			uint32_t _stack_pointer_index;
			uint32_t _code_stack_pointer_index;

            
            void process_include(const std::string &filename, std::istream &ss);
            // Preprocessor
            void process_preprocessor_line(std::vector<std::string> &line);
            void preprocess_normal_line(std::vector<std::string> &line);
            void process_normal_line(std::vector<std::string> &line);
            void add_symbol(const std::string &key, const std::string &value);

            // Assembler
            void add_label(const std::string &label);

            void writeU(uint32_t value);
            void writeI(int32_t value);
            void writeF(float value);
            void writeL(const std::string &label);
            void writeS(const std::string &str);

            void write_setup_code();

            class NumberResult
            {
                public:
                    inline NumberResult(bool is_float, bool is_hex, bool is_number, bool is_error) :
                        _is_float(is_float),
                        _is_hex(is_hex),
                        _is_number(is_number),
                        _is_error(is_error)
                    {
                    }

                    inline bool is_float() const { return _is_float; }
                    inline bool is_hex() const { return _is_hex; }
                    inline bool is_number() const { return _is_number; }
                    inline bool is_error() const { return _is_error; }

                private:
                    bool _is_float;
                    bool _is_hex;
                    bool _is_number;
                    bool _is_error;
            };
            
            static Opcode::Type get_token_type(const std::string &token, bool is_first_token);
            static bool is_register(const std::string &token, std::size_t index);
            static NumberResult is_number(const std::string &token, std::size_t index);
            static uint32_t get_register_value(const std::string &value);
            static uint32_t get_memory_int(const std::string &value);
            static std::string process_label(const std::string &token);

            static std::string process_string_value(const std::string &str);
    };
}
