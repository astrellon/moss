#pragma once

#include "debug_data.h"

namespace moss
{
    class CpuArm;

    class Debugger
    {
        public:
            Debugger(CpuArm *cpu, uint32_t program_offset);

            virtual void on_break();

            virtual void breakpoint_line(const std::string &filename, uint32_t line_number, bool set);
            virtual bool breakpoint_line(const std::string &filename, uint32_t line_number) const;
            virtual void breakpoint(uint32_t program_index, bool set);
            virtual bool breakpoint(uint32_t program_index) const;

            void load_debug_data(std::istream &ss);

        protected:
            CpuArm *_cpu;

            uint32_t _program_offset;
            DebugData _debug_data;

    };
}
