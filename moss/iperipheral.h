#pragma once

#include <stdint.h>

#include "memory.h"

namespace moss
{
    class IPeripheral
    {
        public:
            virtual uint32_t send_command(uint32_t command) = 0;

            virtual void assign_memory(Memory *memory, uint32_t offset, uint32_t size);
            virtual bool assigned() const;

        protected:
            Memory *_memory;
            uint32_t _memory_offset;
            uint32_t _memory_size;

            // INT
            FORCEINLINE int32_t int_data(uint32_t index) const
            {
                return _memory->int_data(index + _memory_offset);
            }
            FORCEINLINE void int_data(uint32_t index, int32_t value)
            {
                _memory->int_data(index + _memory_offset, value);
            }
            // UINT
            FORCEINLINE uint32_t uint_data(uint32_t index) const
            {
                return _memory->uint_data(index + _memory_offset);
            }
            FORCEINLINE void uint_data(uint32_t index, uint32_t value)
            {
                _memory->uint_data(index + _memory_offset, value);
            }
            // FLOAT
            FORCEINLINE float float_data(uint32_t index) const
            {
                return _memory->float_data(index + _memory_offset);
            }
            FORCEINLINE void float_data(uint32_t index, float value)
            {
                _memory->float_data(index + _memory_offset, value);
            }

    };
}
