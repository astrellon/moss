#include "debug_data.h"

#include <moss/utils/tokeniser.h>

namespace moss
{
    DebugData::DebugData() :
        _deserialise_state(READ_NONE)
    {

    }
    
    DebugData::LineData::LineData() :
        _filename_index(-1u),
        _line_number(-1u)
    {

    }
    DebugData::LineData::LineData(uint32_t filename_index, uint32_t line_number) :
        _filename_index(filename_index),
        _line_number(line_number)
    {

    }

    uint32_t DebugData::LineData::filename_index() const
    {
        return _filename_index;
    }
    uint32_t DebugData::LineData::line_number() const
    {
        return _line_number;
    }

    bool DebugData::LineData::operator<(const DebugData::LineData &rhs) const
    {
        if (_filename_index == rhs._filename_index)
        {
            return _line_number < rhs._line_number;
        }
        return _filename_index < rhs._filename_index;
    }


    void DebugData::data(uint32_t program_index, uint32_t line_number, const std::string &filename)
    {
        uint32_t filename_index = find_filename(filename); 
        if (filename_index == -1u)
        {
            _filenames.push_back(filename);
            filename_index = _filenames.size();
        }

        auto data = LineData(filename_index, line_number);
        _prog_to_files[program_index] = data;
        _files_to_prog[data] = program_index;
    }
    void DebugData::data(uint32_t program_index, const LineData &data)
    {
        _prog_to_files[program_index] = data;
        _files_to_prog[data] = program_index;
    }

    DebugData::LineData DebugData::data(uint32_t program_index) const
    {
        auto find = _prog_to_files.find(program_index);
        if (find == _prog_to_files.end())
        {
            return LineData();
        }
        return find->second;
    }
    uint32_t DebugData::data(const DebugData::LineData &data) const
    {
        auto find = _files_to_prog.find(data);
        if (find == _files_to_prog.end())
        {
            return -1u;
        }
        return find->second;
    }
    uint32_t DebugData::data(const std::string &filename, uint32_t line_number) const
    {
        auto filename_index = find_filename(filename);
        if (filename_index == -1u)
        {
            return -1u;
        }
        return data(LineData(filename_index,  line_number));
    }

    const std::vector<std::string> &DebugData::filenames() const
    {
        return _filenames;
    }

    void DebugData::to_stream(std::ostream &ss)
    {
        ss  << "; Debug output:\n"
            "\n"
            "; A list of filenames\n"
            "Filenames:\n";
        for (auto iter = _filenames.begin(); iter != _filenames.end(); ++iter)
        {
            ss << *iter << '\n';
        }

        ss << "\n"
            "; A list of breakpoints to set on files and line number.\n"
            "; The filename is an index to the file list above.\n"
            "Data:\n";
        for (auto iter = _prog_to_files.begin(); iter != _prog_to_files.end(); ++iter)
        {
            ss  << iter->first << ' '
                << iter->second.filename_index() << ' ' 
                << iter->second.line_number() << '\n';
        }
    }
    void DebugData::from_stream(std::istream &ss)
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

    void DebugData::read_data(const std::vector<std::string> &line)
    {
        if (line.size() < 3)
        {
            std::cout << "Line has less than 3 tokens!\n";
            return;
        }

        auto program_index = static_cast<uint32_t>(atoi(line[0].c_str()));
        auto filename_index  = static_cast<uint32_t>(atoi(line[1].c_str()));
        auto line_number = static_cast<uint32_t>(atoi(line[2].c_str()));

        data(program_index, LineData(filename_index, line_number)); 
    }

    uint32_t DebugData::find_filename(const std::string &filename) const
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
}
