#include "debug_config.h"

#include <moss/debug/debugger.h>

namespace moss
{
    DebugConfig::DebugConfig(const std::string &filename) :
        _filename(filename)
    {

    }

    void DebugConfig::add_break_line(uint32_t line_number)
    {
        _break_lines.push_back(line_number);
    }

    void DebugConfig::apply_to_debugger(Debugger *debugger)
    {
        for (auto i = 0u; i < _break_lines.size(); i++)
        {
            debugger->breakpoint_line(_filename, _break_lines[i]);
        }
    }

    const std::string &DebugConfig::filename() const
    {
        return _filename;
    }
}
