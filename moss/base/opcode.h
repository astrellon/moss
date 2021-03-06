#pragma once

#include <map>
#include <vector>
#include <string>

#include <stdint.h>

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

                // MOV {{{
                MOV_R_R,
                MOV_R_I,
                
                MOV_M_R,
                MOV_R_M,
                MOV_M_I,
                MOV_M_M,

                MOV_IM_R,
                MOV_R_IM,
                MOV_IM_I,
                MOV_IM_IM,

                MOV_IM_M,
                MOV_M_IM,

                MOV_R_F,
                MOV_F_R,
                MOV_F_I,

                MOV_R_NR,
                MOV_NR_R,
                MOV_NR_I,
                // }}}

                // Unit conversions {{{
                INT_FLOAT_R,
                INT_FLOAT_R_R,
                FLOAT_INT_R,
                FLOAT_INT_R_R,
                // }}}

                // Stack {{{
                PUSH_R,
                PUSH_I,
                POP_R,
                // }}}

                // CMP/CMPF {{{
                CMP_R_R,
                CMP_R_I,
                CMP_I_R,

                CMPF_R_R,
                CMPF_R_I,
                CMPF_I_R,
                // }}}

                // Branching commands {{{
                
                // JMP {{{
                JMP_R,
                JMP_I,
                // }}}

                // Function commands {{{
                CALL_I,
                CALL_R,
                RETURN,
                // }}}
                
                // }}}

                // Math commands {{{
                
                // ADD/ADDF {{{
                ADD_R_R,
                ADD_R_R_R,
                ADD_R_I_R,
                ADD_R_R_I,
                ADD_R_I,

                ADDF_R_R,
                ADDF_R_R_R,
                ADDF_R_I_R,
                ADDF_R_R_I,
                ADDF_R_I,
                // }}}

                // SUB/SUBF {{{
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
                // }}}

                // INC/DEC {{{
                INC_R,
                INCF_R,
                DEC_R,
                DECF_R,
                // }}}

                // MUL/MULF {{{
                MUL_R_R,
                MUL_R_R_R,
                MUL_R_I_R,
                MUL_R_R_I,
                MUL_R_I,

                MULF_R_R,
                MULF_R_R_R,
                MULF_R_I_R,
                MULF_R_R_I,
                MULF_R_I,
                // }}}

                // DIV/DIVF {{{
                DIV_R_R,
                DIV_R_R_R,
                DIV_R_I_R,
                DIV_R_R_I,
                DIV_R_I,

                DIVF_R_R,
                DIVF_R_R_R,
                DIVF_R_I_R,
                DIVF_R_R_I,
                DIVF_R_I,
                // }}}

                // ROR/ROL {{{
                ROR_R,
                ROR_R_R,
                ROL_R,
                ROL_R_R,
                // }}}

                // SHR/SHL {{{
                SHR_R,
                SHR_R_R,
                SHL_R,
                SHL_R_R,
                // }}}
                
                // AND {{{
                AND_R_R,
                AND_R_R_R,
                AND_R_I_R,
                AND_R_R_I,
                AND_R_I,
                // }}}
                
                // OR {{{
                OR_R_R,
                OR_R_R_R,
                OR_R_I_R,
                OR_R_R_I,
                OR_R_I,
                // }}}
                
                // XOR {{{
                XOR_R_R,
                XOR_R_R_R,
                XOR_R_I_R,
                XOR_R_R_I,
                XOR_R_I,
                // }}}
                
                // NOT {{{
                NOT_R,
                NOT_R_R,
                // }}}

                // Peripherals {{{
                
                // IO_SEND {{{
                IO_SEND_I_I,
                IO_SEND_R_I,
                IO_SEND_I_R,
                IO_SEND_R_R,

                IO_SEND_R_I_I,
                IO_SEND_R_R_I,
                IO_SEND_R_I_R,
                IO_SEND_R_R_R,
                // }}}

                // IO_ASSIGN {{{
                IO_ASSIGN_I_I_I,
                IO_ASSIGN_I_I_R,
                IO_ASSIGN_I_R_I,
                IO_ASSIGN_I_R_R,
                IO_ASSIGN_R_I_I,
                IO_ASSIGN_R_I_R,
                IO_ASSIGN_R_R_I,
                IO_ASSIGN_R_R_R,
                // }}}

                // }}}
                
                // Interrupt commands {{{
                
                // Register interrupt {{{
                REGI_I_I,
                REGI_I_R,
                REGI_R_I,
                REGI_R_R,
                // }}}
                
                INT_I,
                INT_R,
                RETI,
                // }}}
            
                // Debugging commands {{{
                // Print to screen {{{
                PRINT_R,
                PRINT_I,
                PRINT_S,
                
                PRINTF_R,
                PRINTF_I,

                INFO_R,
                // }}}

                // Get input from keyboad {{{
                INPUT_R,
                INPUTF_R,
                // }}}
                
                // Get the time {{{
                TIME_R,
                // }}}
                
                // }}}
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
                INT_MEMORY,
                LABEL,
                NUMBER,
                CONDITION,
                FLAG,
                NAMED_REGISTER,
                STRING
            };
            // }}}
            
            // List of Conditionals {{{
            enum Conditionals
            {
                COND_UNKNOWN = 0x0,
                COND_NONE    = 0x1,
                COND_ANY     = 1 << 28,
                COND_EQ      = 2 << 28,
                COND_NE      = 3 << 28,
                COND_LT      = 4 << 28,
                COND_LE      = 5 << 28,
                COND_GT      = 6 << 28,
                COND_GE      = 7 << 28,
                COND_ALL     = 0xF0000000,
                COND_BREAK   = 0x08000000
            };
            // }}}

            enum NamedRegister
            {
                NAMED_UNKNOWN,
                STACK_POINTER,
                CODE_STACK_POINTER,
                PROGRAM_COUNTER,
                PAGE_TABLE_POINTER
            };

            static Command find_command(const std::string &command_name);
            static std::string build_command_name(const std::string &command, 
                    const std::vector<Type> &types);

            static std::pair<std::string, std::vector<Type> > get_opcode_types(Command command);
            static std::string type_name(Type type);

            static Conditionals find_conditional(const std::string &str);
            static std::string conditional_name(Conditionals cond);
            static std::string conditional_name(uint32_t cond);

            static NamedRegister find_named_register(const std::string &str);
            static std::string named_register_name(NamedRegister reg);
            static std::string named_register_name(uint32_t reg);
            
        private:
            static std::map<std::string, Command> s_names_to_commands;
            static std::map<Command, std::pair<std::string, std::vector<Type> > > s_commands_to_types;
            static std::map<Type, std::string> s_type_names;
            static std::map<Type, std::string> s_type_codes;
            static std::map<std::string, Conditionals> s_conditional_suffix;
            static std::map<std::string, NamedRegister> s_names_to_named_reg;
    };
}
