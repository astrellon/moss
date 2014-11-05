#pragma once

#include <map>
#include <vector>
#include <string>

namespace moss
{
    class Opcode
    {
        public:

            // List of codes {{{
            enum Command
            {
                UNKNOWN_COMMAND,
                HALT,

                MOV_R_R,
                MOV_R_I,
                MOV_M_R,
                MOV_R_M,
                MOV_M_I,
                MOV_M_M,

                UINT_FLOAT_R,
                UINT_FLOAT_R_R,
                FLOAT_UINT_R,
                FLOAT_UINT_R_R,

                PUSH_R,
                PUSH_I,
                POP_R,

                CMP_R_R,
                CMP_R_I,
                CMP_I_R,

                CMPF_R_R,
                CMPF_R_I,
                CMPF_I_R,

                JMP_R,
                JMP_I,
                JNE_R,
                JNE_I,
                JEQ_R,
                JEQ_I,
                JLT_R,
                JLT_I,
                JLE_R,
                JLE_I,
                JGT_R,
                JGT_I,
                JGE_R,
                JGE_I,

                ADD_R_R,
                ADD_R_R_R,
                ADD_R_R_I,
                ADD_R_I,

                ADDF_R_R,
                ADDF_R_R_R,
                ADDF_R_R_I,
                ADDF_R_I,

                SUB_R_R,
                SUB_R_R_R,
                SUB_R_I_R,
                SUB_R_R_I,
                SUB_R_I,

                SUBF_R_R,
                SUBF_R_R_R,
                SUBF_R_I_R,
                SUBF_R_R_I,
                SUBF_R_I,

                INC_R,
                INCF_R,
                DEC_R,
                DECF_R,

                PRINT_R
            };
            // }}}

            // List of argument types {{{
            enum Type
            {
                UNKNOWN_TYPE,
                COMMAND,
                INT_NUMBER,
                FLOAT_NUMBER,
                REGISTER,
                MEMORY,
                LABEL,
                NUMBER
            };
            // }}}

            static Command find_command(const std::string &command_name);
            static std::string build_command_name(const std::string &command, 
                    const std::vector<Type> &types);

            static std::pair<std::string, std::vector<Type> > get_opcode_types(Command command);
            static std::string type_name(Type type);
            
        private:
            static std::map<std::string, Command> s_names_to_commands;
            static std::map<Command, std::pair<std::string, std::vector<Type> > > s_commands_to_types;
            static std::map<Type, std::string> s_type_names;
            static std::map<Type, std::string> s_type_codes;
    };
}
