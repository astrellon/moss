#pragma once

#include <vector>
#include <map>
#include <string>
#include <stdint.h>

#include <moss/debug_common/debug_data.h>

namespace moss
{
    class Debugger;

    class DebugConfig
    {
        public:
            DebugConfig();

            void breakpoint_line(const std::string &filename, uint32_t line_number, bool set);
            bool breakpoint_line(const std::string &filename, uint32_t line_number) const;

            void apply_to_debugger(Debugger *debugger);
    
            const std::vector<std::string> &filenames() const;

            void to_stream(std::ostream &ss);
            void from_stream(std::istream &ss);
        private:
            enum DeserialiseState
            {
                READ_NONE,
                READ_FILENAMES,
                READ_DATA
            };

            void breakpoint_line(uint32_t filename_index, uint32_t line_number, bool set);
            bool breakpoint_line(uint32_t filename_index, uint32_t line_number) const;

            std::vector<std::string> _filenames;
            std::map<uint32_t, std::map<uint32_t, bool> > _file_breakpoints;

            DeserialiseState _deserialise_state;

            uint32_t find_filename(const std::string &filename) const;
            
            void read_data(const std::vector<std::string> &line);

    };
}
