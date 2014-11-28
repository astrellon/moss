#include "debug_config.h"

#include <moss/debug/debugger.h>
#include <moss/utils/tokeniser.h>

namespace moss
{
    DebugConfig::DebugConfig()
    {

    }

    void DebugConfig::breakpoint_line(const std::string &filename, uint32_t line_number, bool set)
    {
        auto filename_index = find_filename(filename);
        if (filename_index == -1u)
        {
            filename_index = _filenames.size();
            _filenames.push_back(filename);
        }

        breakpoint_line(filename_index, line_number, set);
    }
    bool DebugConfig::breakpoint_line(const std::string &filename, uint32_t line_number) const
    {
        auto filename_index = find_filename(filename);
        if (filename_index == -1u)
        {
            return false;
        }
        return breakpoint_line(filename_index, line_number);
    }
    void DebugConfig::breakpoint_line(uint32_t filename_index, uint32_t line_number, bool set)
    {
        _file_breakpoints[filename_index][line_number] = set;
    }
    bool DebugConfig::breakpoint_line(uint32_t filename_index, uint32_t line_number) const
    {
        auto find_file = _file_breakpoints.find(filename_index);
        if (find_file == _file_breakpoints.end())
        {
            return false;
        }

        auto find_line = find_file->second.find(line_number);
        if (find_line == find_file->second.end())
        {
            return false;
        }

        return find_line->second;
    }

    void DebugConfig::apply_to_debugger(Debugger *debugger)
    {
        for (auto file_iter : _file_breakpoints)
        {
            for (auto line_iter : file_iter.second)
            {
                debugger->breakpoint_line(_filenames[file_iter.first], line_iter.first, line_iter.second);
            }
        }
    }

    void DebugConfig::to_stream(std::ostream &ss)
    {
        ss  << "; Debug breakpoints output:\n"
            "\n"
            "; A list of filenames\n"
            "Filenames:\n";
        for (auto iter = _filenames.begin(); iter != _filenames.end(); ++iter)
        {
            ss << *iter << '\n';
        }

        ss << "\n"
            "; The data here is made up of the index of the instruction in the program,\n"
            "; the filename index (based on the list of filenames above) and the line number.\n"
            "Data:\n";
        for (auto file_iter : _file_breakpoints)
        {
            for (auto line_iter : file_iter.second)
            {
                if (line_iter.second)
                {
                    ss << file_iter.first << ' ' << line_iter.first << '\n';
                }
            }
        }
    }
    void DebugConfig::from_stream(std::istream &ss)
    {
        auto tokens = new Tokeniser(ss, false);

        while (tokens->has_tokens())
        {
            auto line = tokens->next_token_line();
            if (line[0] == "Filenames:")
            {
                _deserialise_state = READ_FILENAMES;
                continue;
            }
            else if (line[0] == "Data:")
            {
                _deserialise_state = READ_DATA;
                continue;
            }

            if (_deserialise_state == READ_FILENAMES)
            {
                _filenames.push_back(line[0]);
            }
            else if (_deserialise_state == READ_DATA)
            {
                read_data(line);
            }
        }  

        delete tokens;
    }
    
    uint32_t DebugConfig::find_filename(const std::string &filename) const
    {
        for (uint32_t i = 0; i < _filenames.size(); i++)
        {
            if (_filenames[i] == filename)
            {
                return i;
            }
        }
        return -1u;
    }

    void DebugConfig::read_data(const std::vector<std::string> &line)
    {
        if (line.size() < 2)
        {
            std::cout << "Line has less than 2 tokens!\n";
            return;
        }

        auto filename_index  = static_cast<uint32_t>(atoi(line[0].c_str()));
        auto line_number = static_cast<uint32_t>(atoi(line[1].c_str()));

        breakpoint_line(filename_index, line_number);
    }

}
