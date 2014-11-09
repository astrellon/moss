#pragma once

#include <map>
#include <string>

namespace moss
{
    class OpcodeArm
    {
        public:

            enum Conditionals
            {
                FLAG_HAS_CONDITION =    0x08000000,
                FLAG_NEGATIVE =         0x10000000,
                FLAG_ZERO =             0x20000000,
                FLAG_CARRY =            0x40000000,
                FLAG_OVERFLOW =         0x80000000,
                FLAG_ALL =              0xF0000000
            };

        private:
            static std::map<std::string, Conditionals> s_conditional_suffix;
    };
}
