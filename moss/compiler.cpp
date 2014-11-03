#include "compiler.h"

#include "common.h"

namespace moss
{
    Compiler::Compiler() :
        _index(0),
        _tokens(nullptr)
    {

    }
    Compiler::~Compiler()
    {
        if (_tokens != nullptr)
        {
            delete _tokens;
        }
        finalise();
    }

    void Compiler::process_stream(const std::string &filename, std::istream &ss)
    {
        _tokens = new Tokeniser(ss);


    }

    bool Compiler::finalise()
    {
        for (auto iter = _label_temp.begin(); iter != _label_temp.end(); ++iter)
        {
            auto find = _label_locations.find(iter->first);
            if (find == _label_locations.end())
            {
                std::cout << "Error finalising! No label defined for: " << iter->first << std::endl;
                return false;
            }

            for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2)
            {
                _data[*iter2] = find->second;
            }
        }

        _label_temp.clear();
        return true;
    }

    void Compiler::add_label(const std::string &label)
    {
        _label_locations[std::string(label)] = _index;
    }

    void Compiler::write(uint32_t v1)
    {
        _data.push_back(v1);
        ++_index;
    }
    void Compiler::write(uint32_t v1, uint32_t v2)
    {
        _data.push_back(v1);
        _data.push_back(v2);
        _index += 2;
    }
    void Compiler::write(uint32_t v1, uint32_t v2, uint32_t v3)
    {
        _data.push_back(v1);
        _data.push_back(v2);
        _data.push_back(v3);
        _index += 3;
    }
    void Compiler::write(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4)
    {
        _data.push_back(v1);
        _data.push_back(v2);
        _data.push_back(v3);
        _data.push_back(v4);
        _index += 4;
    }

    void Compiler::writeF(uint32_t v1, float v2)
    {
        _data.push_back(v1);
        _data.push_back(TO_UINT(v2));
        _index += 2;
    }
    void Compiler::writeF(uint32_t v1, float v2, float v3)
    {
        _data.push_back(v1);
        _data.push_back(TO_UINT(v2));
        _data.push_back(TO_UINT(v3));
        _index += 3;
    }
    void Compiler::writeF(uint32_t v1, float v2, float v3, float v4)
    {
        _data.push_back(v1);
        _data.push_back(TO_UINT(v2));
        _data.push_back(TO_UINT(v3));
        _data.push_back(TO_UINT(v4));
        _index += 4;
    }

    void Compiler::write(uint32_t v1, const char *label)
    {
        _data.push_back(v1);
        _label_temp[std::string(label)].push_back(_index + 1);
        _data.push_back(0u);
        _index += 2;
    }
}
