#pragma once

#include <stdint.h>

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

            void write(uint32_t v1)
            {
                _memory->data(_index++, v1);
            }
            void write(uint32_t v1, uint32_t v2)
            {
                _memory->data(_index++, v1);
                _memory->data(_index++, v2);
            }
            void write(uint32_t v1, uint32_t v2, uint32_t v3)
            {
                _memory->data(_index++, v1);
                _memory->data(_index++, v2);
                _memory->data(_index++, v3);
            }
            void write(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4)
            {
                _memory->data(_index++, v1);
                _memory->data(_index++, v2);
                _memory->data(_index++, v3);
                _memory->data(_index++, v4);
            }

        private:
            uint32_t _index;
            T *_memory;
    };
}
