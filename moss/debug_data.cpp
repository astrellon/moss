#include "debug_data.h"

#include "tokeniser.h"

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


    void DebugData::data(uint32_t program_index, uint32_t line_number, const std::string &filename)
    {
        uint32_t filename_index = find_filename(filename); 
        if (filename_index == -1u)
        {
            _filenames.push_back(filename);
            filename_index = _filenames.size();
        }

        _data[program_index] = LineData(filename_index, line_number);
    }
    void DebugData::data(uint32_t program_index, const LineData &data)
    {
        _data[program_index] = data;
    }

    DebugData::LineData DebugData::data(uint32_t program_index)
    {
        auto find = _data.find(program_index);
        if (find == _data.end())
        {
            return LineData();
        }
        return find->second;
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
            "; The data here is made up of the index of the instruction in the program,\n"
            "; the filename index (based on the list of filenames above) and the line number.\n"
            "Data:\n";
        for (auto iter = _data.begin(); iter != _data.end(); ++iter)
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
