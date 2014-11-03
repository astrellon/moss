#pragma once

#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "common.h"

namespace moss
{
    template <class T>
    class MemoryWriter
    {
        public:
            MemoryWriter(uint32_t index, T *memory) :
                _index(index),
                _memory(memory)
            {

            }
            ~MemoryWriter()
            {
                finalise();
            }

            bool finalise()
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
                        _memory->uint_data(*iter2, find->second);
                    }
                }

                _label_temp.clear();
                return true;
            }

            void add_label(const char *label)
            {
                _label_locations[std::string(label)] = _index;
            }

            void write(uint32_t v1, const char *label)
            {
                _memory->uint_data(_index++, v1);
                _label_temp[std::string(label)].push_back(_index);
                _memory->uint_data(_index++, 0u);
            }

            void write(uint32_t v1)
            {
                _memory->uint_data(_index++, v1);
            }
            void write(uint32_t v1, uint32_t v2)
            {
                _memory->uint_data(_index++, v1);
                _memory->uint_data(_index++, v2);
            }
            void write(uint32_t v1, uint32_t v2, uint32_t v3)
            {
                _memory->uint_data(_index++, v1);
                _memory->uint_data(_index++, v2);
                _memory->uint_data(_index++, v3);
            }
            void write(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4)
            {
                _memory->uint_data(_index++, v1);
                _memory->uint_data(_index++, v2);
                _memory->uint_data(_index++, v3);
                _memory->uint_data(_index++, v4);
            }
            
            void writeF(float v1)
            {
                _memory->float_data(_index++, v1);
            }
            void writeF(uint32_t v1, float v2)
            {
                _memory->uint_data(_index++, v1);
                _memory->float_data(_index++, v2);
            }
            void writeF(uint32_t v1, float v2, float v3)
            {
                _memory->uint_data(_index++, v1);
                _memory->float_data(_index++, v2);
                _memory->float_data(_index++, v3);
            }
            void writeF(uint32_t v1, float v2, float v3, float v4)
            {
                _memory->uint_data(_index++, v1);
                _memory->float_data(_index++, v2);
                _memory->float_data(_index++, v3);
                _memory->float_data(_index++, v4);
            }

        private:
            uint32_t _index;
            T *_memory;
            std::map< std::string, uint32_t > _label_locations;
            std::map< std::string, std::vector<uint32_t> > _label_temp;
    };
}
