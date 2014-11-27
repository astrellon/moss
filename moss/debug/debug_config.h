#pragma once

#include <vector>
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

            void add_break_line(uint32_t line_number);

            void apply_to_debugger(Debugger *debugger);
    
        private:
            DebugData _debug_data;

    };
}
