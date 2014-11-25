#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <stdint.h>

namespace moss
{
    class DebugData
    {
        public:
            DebugData();
            
            class LineData
            {
                public:
                    LineData();
                    LineData(uint32_t filename_index, uint32_t line_number);

                    uint32_t filename_index() const;
                    uint32_t line_number() const;

                private:
                    uint32_t _filename_index;
                    uint32_t _line_number;
            };


            void data(uint32_t program_index, uint32_t line_number, const std::string &filename);
            LineData data(uint32_t program_index);

            const std::vector<std::string> &filenames() const;

            void to_stream(std::ostream &ss);
            void from_stream(std::istream &ss);
        private:

            void data(uint32_t program_index, const LineData &data);

            enum DeserialiseState
            {
                READ_NONE,
                READ_FILENAMES,
                READ_DATA
            };

            std::vector<std::string> _filenames;
            std::map<uint32_t, LineData> _data;
            DeserialiseState _deserialise_state;

            uint32_t find_filename(const std::string &filename) const;

            void read_data(const std::vector<std::string> &line);
    };
}
