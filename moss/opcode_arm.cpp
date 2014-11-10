#include "opcode_arm.h"

namespace moss
{
    std::map<std::string, OpcodeArm::Conditionals> OpcodeArm::s_conditional_suffix = {
        { "EQ", OpcodeArm::COND_EQ },
        { "==", OpcodeArm::COND_EQ },
        { "NE", OpcodeArm::COND_NE },
        { "!=", OpcodeArm::COND_NE },

        { "LT", OpcodeArm::COND_LT },
        { "<",  OpcodeArm::COND_LT },
        { "LE", OpcodeArm::COND_LE },
        { "<=", OpcodeArm::COND_LE },
        
        { "GT", OpcodeArm::COND_GT },
        { ">",  OpcodeArm::COND_GT },
        { "GE", OpcodeArm::COND_GE },
        { ">=", OpcodeArm::COND_GE }
    };
    OpcodeArm::Conditionals OpcodeArm::get_conditional(const std::string &str)
    {
        auto find = s_conditional_suffix.find(str);
        if (find == s_conditional_suffix.end())
        {
            return COND_UNKNOWN;
        }

        return find->second;
    }
    std::string OpcodeArm::get_conditional(uint32_t cond)
    {
        for (auto iter = s_conditional_suffix.begin(); iter != s_conditional_suffix.end(); ++iter)
        {
            if (iter->second == cond)
            {
                return iter->first;
            }
        }
        return std::string("Unknown conditional");
    }
}
