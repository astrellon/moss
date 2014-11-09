#include "opcode_arm.h"

namespace moss
{
    std::map<std::string, OpcodeArm::Conditionals> s_conditional_suffix = {
        { "EQ", OpcodeArm::FLAG_ZERO },
        { "==", OpcodeArm::FLAG_ZERO },
        /*
        { "NE", 0 },
        { "!=", 0 },
        */

        { "LT", OpcodeArm::FLAG_NEGATIVE },
        { "<", OpcodeArm::FLAG_NEGATIVE },
        { "LT", OpcodeArm::FLAG_NEGATIVE },
        { "<", OpcodeArm::FLAG_NEGATIVE }
    };
}
