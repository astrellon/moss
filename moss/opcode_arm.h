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
                COND_UNKNOWN =      0x0,
                COND_NONE =         0x1,
                COND_ANY =           1 << 28, 
                COND_EQ =            2 << 28, 
                COND_NE =            3 << 28, 
                COND_LT =            4 << 28, 
                COND_LE =            5 << 28, 
                COND_GT =            6 << 28, 
                COND_GE =            7 << 28,
                COND_ALL =          0xF0000000
            };

            static Conditionals get_conditional(const std::string &str);
            static std::string get_conditional(uint32_t cond);

        private:
            static std::map<std::string, Conditionals> s_conditional_suffix;
    };
}
