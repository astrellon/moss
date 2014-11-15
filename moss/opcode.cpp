#include "opcode.h"

#include <sstream>

namespace moss
{
    Opcode::Command Opcode::find_command(const std::string &command_name)
    {
        auto find = s_names_to_commands.find(command_name);
        if (find == s_names_to_commands.end())
        {
            return  UNKNOWN_COMMAND;
        }
        return find->second;
    }

    std::string Opcode::build_command_name(const std::string &command, 
            const std::vector<Opcode::Type> &types)
    {
        std::stringstream ss;    
        ss << command;
        for (auto iter = types.begin(); iter != types.end(); ++iter)
        {
            ss << '_';
            ss << s_type_codes[*iter];
        }
        return ss.str();
    }

    std::string Opcode::type_name(Type type)
    {
        return s_type_names[type];
    }
    std::pair<std::string, std::vector<Opcode::Type> > Opcode::get_opcode_types(Opcode::Command command)
    {
        return s_commands_to_types[command];
    }
    
    Opcode::Conditionals Opcode::find_conditional(const std::string &str)
    {
        auto find = s_conditional_suffix.find(str);
        if (find == s_conditional_suffix.end())
        {
            return COND_UNKNOWN;
        }

        return find->second;
    }
    std::string Opcode::conditional_name(Opcode::Conditionals cond)
    {
        return conditional_name(static_cast<uint32_t>(cond));
    }
    std::string Opcode::conditional_name(uint32_t cond)
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

    Registers::Flags Opcode::find_flag(const std::string &str)
    {
        auto find = s_names_to_flags.find(str);
        if (find != s_names_to_flags.end())
        {
            return find->second;
        }
        return Registers::FLAG_UNKNOWN;
    }
    std::string Opcode::flag_name(Registers::Flags flag)
    {
        return flag_name(static_cast<uint32_t>(flag));
    }
    std::string Opcode::flag_name(uint32_t flag)
    {
        for (auto iter = s_names_to_flags.begin(); iter != s_names_to_flags.end(); ++iter)
        {
            if (iter->second == flag)
            {
                return iter->first;
            }
        }
        return std::string("Unknown flag");
    }

    Opcode::NamedRegister Opcode::find_named_register(const std::string &str)
    {
        auto find = s_names_to_named_reg.find(str);
        if (find != s_names_to_named_reg.end())
        {
            return find->second;
        }
        return Opcode::NAMED_UNKNOWN;
    }
    std::string Opcode::named_register_name(Opcode::NamedRegister flag)
    {
        return flag_name(static_cast<uint32_t>(flag));
    }
    std::string Opcode::named_register_name(uint32_t flag)
    {
        for (auto iter = s_names_to_named_reg.begin(); iter != s_names_to_named_reg.end(); ++iter)
        {
            if (iter->second == flag)
            {
                return iter->first;
            }
        }
        return std::string("Unknown named register");
    }

    // This is used by the assembler once the command and argument types have been
    // determined to get the final opcode.
	std::map<std::string, Opcode::Command> Opcode::s_names_to_commands = {
		{ std::string("HALT"), Opcode::HALT },

		// MOV {{{
		{ std::string("MOV_R_R"), Opcode::MOV_R_R },
		{ std::string("MOV_R_I"), Opcode::MOV_R_I },
		{ std::string("MOV_M_R"), Opcode::MOV_M_R },

		{ std::string("MOV_R_M"), Opcode::MOV_R_M },
		{ std::string("MOV_M_I"), Opcode::MOV_M_I },
		{ std::string("MOV_M_M"), Opcode::MOV_M_M },

		{ std::string("MOV_R_F"), Opcode::MOV_R_F },
		{ std::string("MOV_F_R"), Opcode::MOV_F_R },
		{ std::string("MOV_F_I"), Opcode::MOV_F_I },
		
        { std::string("MOV_R_NR"), Opcode::MOV_R_NR },
        { std::string("MOV_NR_R"), Opcode::MOV_NR_R },
        { std::string("MOV_NR_I"), Opcode::MOV_NR_I },
		// }}}

		// Unit conversions {{{
		{ std::string("UINT_FLOAT_R"),   Opcode::UINT_FLOAT_R },
		{ std::string("UINT_FLOAT_R_R"), Opcode::UINT_FLOAT_R_R },
		{ std::string("FLOAT_UINT_R"),   Opcode::FLOAT_UINT_R },
		{ std::string("FLOAT_UINT_R_R"), Opcode::FLOAT_UINT_R_R },
		// }}}

		// Stack {{{
		{ std::string("PUSH_R"), Opcode::PUSH_R },
		{ std::string("PUSH_I"), Opcode::PUSH_I },
		{ std::string("POP_R"),  Opcode::POP_R },
		// }}}

		// CMP/CMPF {{{
		{ std::string("CMP_R_R"), Opcode::CMP_R_R },
		{ std::string("CMP_R_I"), Opcode::CMP_R_I },
		{ std::string("CMP_I_R"), Opcode::CMP_I_R },

		{ std::string("CMPF_R_I"), Opcode::CMPF_R_I },
		{ std::string("CMPF_I_R"), Opcode::CMPF_I_R },
		// }}}

		// Branching {{{
		{ std::string("JMP_R"), Opcode::JMP_R },
		{ std::string("JMP_I"), Opcode::JMP_I },
		// }}}

		// ADD/ADDF {{{
		{ std::string("ADD_R_R"),   Opcode::ADD_R_R },
		{ std::string("ADD_R_R_R"), Opcode::ADD_R_R_R },
		{ std::string("ADD_R_R_I"), Opcode::ADD_R_R_I },
		{ std::string("ADD_R_I"),   Opcode::ADD_R_I },

		{ std::string("ADDF_R_R"),   Opcode::ADDF_R_R },
		{ std::string("ADDF_R_R_R"), Opcode::ADDF_R_R_R },
		{ std::string("ADDF_R_R_I"), Opcode::ADDF_R_R_I },
		{ std::string("ADDF_R_I"),   Opcode::ADDF_R_I },
		// }}}

		// SUB/SUBF {{{
		{ std::string("SUB_R_R"),   Opcode::SUB_R_R },
		{ std::string("SUB_R_R_R"), Opcode::SUB_R_R_R },
		{ std::string("SUB_R_I_R"), Opcode::SUB_R_I_R },
		{ std::string("SUB_R_R_I"), Opcode::SUB_R_R_I },
		{ std::string("SUB_R_I"),   Opcode::SUB_R_I },

		{ std::string("SUBF_R_R"),   Opcode::SUBF_R_R },
		{ std::string("SUBF_R_R_R"), Opcode::SUBF_R_R_R },
		{ std::string("SUBF_R_I_R"), Opcode::SUBF_R_I_R },
		{ std::string("SUBF_R_R_I"), Opcode::SUBF_R_R_I },
		{ std::string("SUBF_R_I"),   Opcode::SUBF_R_I },
		// }}}

		// INC/DEC {{{
		{ std::string("INC_R"),  Opcode::INC_R },
		{ std::string("INCF_R"), Opcode::INCF_R },
		{ std::string("DEC_R"),  Opcode::DEC_R },
		{ std::string("DECF_R"), Opcode::DECF_R },
		// }}}

		// MUL/MULF {{{
		{ std::string("MUL_R_R"),   Opcode::MUL_R_R },
		{ std::string("MUL_R_R_R"), Opcode::MUL_R_R_R },
		{ std::string("MUL_R_R_I"), Opcode::MUL_R_R_I },
		{ std::string("MUL_R_I"),   Opcode::MUL_R_I },

		{ std::string("MULF_R_R"),   Opcode::MULF_R_R },
		{ std::string("MULF_R_R_R"), Opcode::MULF_R_R_R },
		{ std::string("MULF_R_R_I"), Opcode::MULF_R_R_I },
		{ std::string("MULF_R_I"),   Opcode::MULF_R_I },
		// }}}

		// DIV/DIVF {{{
		{ std::string("DIV_R_R"),   Opcode::DIV_R_R },
		{ std::string("DIV_R_R_R"), Opcode::DIV_R_R_R },
		{ std::string("DIV_R_I_R"), Opcode::DIV_R_I_R },
		{ std::string("DIV_R_R_I"), Opcode::DIV_R_R_I },
		{ std::string("DIV_R_I"),   Opcode::DIV_R_I },

		{ std::string("DIVF_R_R"),   Opcode::DIVF_R_R },
		{ std::string("DIVF_R_R_R"), Opcode::DIVF_R_R_R },
		{ std::string("DIVF_R_I_R"), Opcode::DIVF_R_I_R },
		{ std::string("DIVF_R_R_I"), Opcode::DIVF_R_R_I },
		{ std::string("DIVF_R_I"),   Opcode::DIVF_R_I },
		// }}}

		// ROR/ROL {{{
		{ std::string("ROR_R"),   Opcode::ROR_R },
		{ std::string("ROR_R_R"), Opcode::ROR_R_R },
		{ std::string("ROL_R"),   Opcode::ROL_R },
		{ std::string("ROL_R_R"), Opcode::ROL_R_R },
		// }}}

		// SHR/SHL {{{
		{ std::string("SHR_R"),   Opcode::SHR_R },
		{ std::string("SHR_R_R"), Opcode::SHR_R_R },
		{ std::string("SHL_R"),   Opcode::SHL_R },
		{ std::string("SHL_R_R"), Opcode::SHL_R_R },
		// }}}

		// Peripherals {{{
		// SEND {{{
		{ std::string("IO_SEND_I_I"), Opcode::IO_SEND_I_I },
        { std::string("IO_SEND_R_I"), Opcode::IO_SEND_R_I },
        { std::string("IO_SEND_I_R"), Opcode::IO_SEND_I_R },
        { std::string("IO_SEND_R_R"), Opcode::IO_SEND_R_R },
        
		{ std::string("IO_SEND_R_I_I"), Opcode::IO_SEND_R_I_I },
        { std::string("IO_SEND_R_R_I"), Opcode::IO_SEND_R_R_I },
        { std::string("IO_SEND_R_I_R"), Opcode::IO_SEND_R_I_R },
        { std::string("IO_SEND_R_R_R"), Opcode::IO_SEND_R_R_R },
		// }}}
        
		// ASSIGN {{{
		{ std::string("IO_ASSIGN_R_R_R"), Opcode::IO_ASSIGN_R_R_R },
		{ std::string("IO_ASSIGN_R_R_I"), Opcode::IO_ASSIGN_R_R_I },
		{ std::string("IO_ASSIGN_R_I_R"), Opcode::IO_ASSIGN_R_I_R },
		{ std::string("IO_ASSIGN_R_I_I"), Opcode::IO_ASSIGN_R_I_I },
		
		{ std::string("IO_ASSIGN_I_R_R"), Opcode::IO_ASSIGN_I_R_R },
		{ std::string("IO_ASSIGN_I_R_I"), Opcode::IO_ASSIGN_I_R_I },
		{ std::string("IO_ASSIGN_I_I_R"), Opcode::IO_ASSIGN_I_I_R },
		{ std::string("IO_ASSIGN_I_I_I"), Opcode::IO_ASSIGN_I_I_I },
        // }}}
        
        // }}}
        
        { std::string("PRINT_R"),  Opcode::PRINT_R }
    };

    // This map is used by the disassembler for converting opcode values back
    // into the original command name with the expected arguments. 
    std::map<Opcode::Command, std::pair<std::string, std::vector<Opcode::Type> > > Opcode::s_commands_to_types = {
        { Opcode::HALT, { "halt", {} } },

        // MOV {{{
        { Opcode::MOV_R_R, { "mov", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::MOV_R_I, { "mov", { Opcode::REGISTER, Opcode::NUMBER } } },
        { Opcode::MOV_M_R, { "mov", { Opcode::MEMORY, Opcode::REGISTER } } },
       
        { Opcode::MOV_R_M, { "mov", { Opcode::REGISTER, Opcode::MEMORY } } },
        { Opcode::MOV_M_I, { "mov", { Opcode::MEMORY, Opcode::NUMBER } } },
        { Opcode::MOV_M_M, { "mov", { Opcode::MEMORY, Opcode::MEMORY } } },
        
        { Opcode::MOV_R_F, { "mov", { Opcode::REGISTER, Opcode::FLAG } } },
        { Opcode::MOV_F_R, { "mov", { Opcode::FLAG, Opcode::REGISTER } } },
        { Opcode::MOV_F_I, { "mov", { Opcode::FLAG, Opcode::INT_NUMBER } } },
        
        { Opcode::MOV_R_NR, { "mov", { Opcode::REGISTER, Opcode::NAMED_REGISTER } } },
        { Opcode::MOV_NR_R, { "mov", { Opcode::NAMED_REGISTER, Opcode::REGISTER } } },
        { Opcode::MOV_NR_I, { "mov", { Opcode::NAMED_REGISTER, Opcode::NUMBER } } },
        // }}}
        
        // Unit conversion {{{
        { Opcode::UINT_FLOAT_R,   { "uint_float", { Opcode::REGISTER } } },
        { Opcode::UINT_FLOAT_R_R, { "uint_float", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::FLOAT_UINT_R,   { "float_uint", { Opcode::REGISTER } } },
        { Opcode::FLOAT_UINT_R_R, { "float_uint", { Opcode::REGISTER, Opcode::REGISTER } } },
        // }}}
        
        // Stack {{{
        { Opcode::PUSH_R, { "push", { Opcode::REGISTER } } },
        { Opcode::PUSH_I, { "push", { Opcode::NUMBER } } },
        { Opcode::POP_R,  { "pop",  { Opcode::REGISTER } } },
        // }}}
        
        // CMP/CMPF {{{
        { Opcode::CMP_R_R, { "cmp", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::CMP_R_I, { "cmp", { Opcode::REGISTER, Opcode::INT_NUMBER } } }, 
        { Opcode::CMP_I_R, { "cmp", { Opcode::INT_NUMBER, Opcode::REGISTER } } },
        
        { Opcode::CMPF_R_R, { "cmpf", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::CMPF_R_I, { "cmpf", { Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        { Opcode::CMPF_I_R, { "cmpf", { Opcode::FLOAT_NUMBER, Opcode::REGISTER } } },
        // }}}
        
        // Branching {{{
        { Opcode::JMP_R, { "jmp", { Opcode::REGISTER } } },
        { Opcode::JMP_I, { "jmp", { Opcode::INT_NUMBER } } },
        // }}}
        
        // ADD/ADDF {{{
        { Opcode::ADD_R_R,    { "add", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::ADD_R_R_R,  { "add", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::ADD_R_R_I,  { "add", { Opcode::REGISTER, Opcode::REGISTER, Opcode::INT_NUMBER } } },
        { Opcode::ADD_R_I,    { "add", { Opcode::REGISTER, Opcode::INT_NUMBER } } },
        
        { Opcode::ADDF_R_R,   { "addf", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::ADDF_R_R_R, { "addf", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::ADDF_R_R_I, { "addf", { Opcode::REGISTER, Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        { Opcode::ADDF_R_I,   { "addf", { Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        // }}}
        
        // SUB/SUBF {{{
        { Opcode::SUB_R_R,    { "sub", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SUB_R_R_R,  { "sub", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SUB_R_I_R,  { "sub", { Opcode::REGISTER, Opcode::INT_NUMBER, Opcode::REGISTER } } },
        { Opcode::SUB_R_R_I,  { "sub", { Opcode::REGISTER, Opcode::REGISTER, Opcode::INT_NUMBER } } },
        { Opcode::SUB_R_I,    { "sub", { Opcode::REGISTER, Opcode::INT_NUMBER } } },
        
        { Opcode::SUBF_R_R,   { "subf", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SUBF_R_R_R, { "subf", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SUBF_R_I_R, { "subf", { Opcode::FLOAT_NUMBER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SUBF_R_R_I, { "subf", { Opcode::REGISTER, Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        { Opcode::SUBF_R_I,   { "subf", { Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        // }}}

        // INC/DEC {{{
        { Opcode::INC_R,  { "inc",  { Opcode::REGISTER } } },
        { Opcode::INCF_R, { "incf", { Opcode::REGISTER } } },
        { Opcode::DEC_R,  { "dec",  { Opcode::REGISTER } } },
        { Opcode::DECF_R, { "decf", { Opcode::REGISTER } } },
        // }}}
        
        // MUL/MULF {{{
        { Opcode::MUL_R_R,    { "mul", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::MUL_R_R_R,  { "mul", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::MUL_R_R_I,  { "mul", { Opcode::REGISTER, Opcode::REGISTER, Opcode::INT_NUMBER } } },
        { Opcode::MUL_R_I,    { "mul", { Opcode::REGISTER, Opcode::INT_NUMBER } } },
        
        { Opcode::MULF_R_R,   { "mulf", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::MULF_R_R_R, { "mulf", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::MULF_R_R_I, { "mulf", { Opcode::REGISTER, Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        { Opcode::MULF_R_I,   { "mulf", { Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        // }}}
        
        // DIV/DIVF {{{
        { Opcode::DIV_R_R,    { "div", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::DIV_R_R_R,  { "div", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::DIV_R_I_R,  { "div", { Opcode::REGISTER, Opcode::INT_NUMBER, Opcode::REGISTER } } },
        { Opcode::DIV_R_R_I,  { "div", { Opcode::REGISTER, Opcode::REGISTER, Opcode::INT_NUMBER } } },
        { Opcode::DIV_R_I,    { "div", { Opcode::REGISTER, Opcode::INT_NUMBER } } },
        
        { Opcode::DIVF_R_R,   { "divf", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::DIVF_R_R_R, { "divf", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::DIVF_R_I_R, { "divf", { Opcode::FLOAT_NUMBER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::DIVF_R_R_I, { "divf", { Opcode::REGISTER, Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        { Opcode::DIVF_R_I,   { "divf", { Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        // }}}
        
        // ROR/ROL {{{
        { Opcode::ROR_R,   { "ror", { Opcode::REGISTER } } },
        { Opcode::ROR_R_R, { "ror", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::ROL_R,   { "rol", { Opcode::REGISTER } } },
        { Opcode::ROL_R_R, { "rol", { Opcode::REGISTER, Opcode::REGISTER } } },
        // }}}
        
        // SHR/SHL {{{
        { Opcode::SHR_R,   { "shr", { Opcode::REGISTER } } },
        { Opcode::SHR_R_R, { "shr", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SHL_R,   { "shl", { Opcode::REGISTER } } },
        { Opcode::SHL_R_R, { "shl", { Opcode::REGISTER, Opcode::REGISTER } } },
        // }}}

        // Peripherals {{{

        // SEND {{{
        { Opcode::IO_SEND_I_I, { "io_send", { Opcode::INT_NUMBER, Opcode::NUMBER } } },
        { Opcode::IO_SEND_R_I, { "io_send", { Opcode::REGISTER, Opcode::NUMBER } } },
        { Opcode::IO_SEND_I_R, { "io_send", { Opcode::INT_NUMBER, Opcode::REGISTER } } },
        { Opcode::IO_SEND_R_R, { "io_send", { Opcode::REGISTER, Opcode::REGISTER } } },
        
        { Opcode::IO_SEND_R_I_I, { "io_send", { Opcode::REGISTER, Opcode::INT_NUMBER, Opcode::NUMBER } } },
        { Opcode::IO_SEND_R_R_I, { "io_send", { Opcode::REGISTER, Opcode::REGISTER, Opcode::NUMBER } } },
        { Opcode::IO_SEND_R_I_R, { "io_send", { Opcode::REGISTER, Opcode::INT_NUMBER, Opcode::REGISTER } } },
        { Opcode::IO_SEND_R_R_R, { "io_send", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        // }}}
        
        // ASSIGN {{{
        { Opcode::IO_ASSIGN_R_R_R, { "io_assign", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::IO_ASSIGN_R_R_I, { "io_assign", { Opcode::REGISTER, Opcode::REGISTER, Opcode::NUMBER } } },
        { Opcode::IO_ASSIGN_R_I_R, { "io_assign", { Opcode::REGISTER, Opcode::NUMBER, Opcode::REGISTER } } },
        { Opcode::IO_ASSIGN_R_I_I, { "io_assign", { Opcode::REGISTER, Opcode::NUMBER, Opcode::NUMBER } } },
        
        { Opcode::IO_ASSIGN_I_R_R, { "io_assign", { Opcode::NUMBER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::IO_ASSIGN_I_R_I, { "io_assign", { Opcode::NUMBER, Opcode::REGISTER, Opcode::NUMBER } } },
        { Opcode::IO_ASSIGN_I_I_R, { "io_assign", { Opcode::NUMBER, Opcode::NUMBER, Opcode::REGISTER } } },
        { Opcode::IO_ASSIGN_I_I_I, { "io_assign", { Opcode::NUMBER, Opcode::NUMBER, Opcode::NUMBER } } },
        // }}}
        
        // }}}

        { Opcode::PRINT_R, { "print", { Opcode::INT_NUMBER } } }
    };
    
    // Converts opcode types to strings. Mostly for debugging the token parsing output.
    std::map<Opcode::Type, std::string> Opcode::s_type_names = {
        { Opcode::UNKNOWN_TYPE, std::string("unknown") },
        { Opcode::COMMAND,      std::string("command") },
        { Opcode::INT_NUMBER,   std::string("int_number") },
        { Opcode::FLOAT_NUMBER, std::string("float_number") },
        { Opcode::REGISTER,     std::string("register") },
        { Opcode::MEMORY,       std::string("memory") },
        { Opcode::LABEL,        std::string("label") },
        { Opcode::NUMBER,       std::string("number") },
        { Opcode::CONDITION,    std::string("condition") },
        { Opcode::FLAG,         std::string("flag") }
    };
    
    // For creating the command lookup key.
    // Each command after being parsed into Opcode parts well then
    // be turned into a string with the COMMAND_TYPEN format.
    // For example:
    // MOV r0 5.6
    // Is turned into
    // COMMAND MOV
    // REGISTER 0
    // FLOAT_NUMBER 5.6
    //
    // Which then uses this lookup for values other than the Command
    // to create the lookup for the s_names_to_commands to get the
    // final opcode for the command.
    //
    // The key could be something else other than a string really,
    // however strings already exist and my special key struct didn't.
    //
    // That is the reason why values that are invalid opcode arguments still
    // have values, just for debugging when an inevitable error occurs.
    std::map<Opcode::Type, std::string> Opcode::s_type_codes = {
        { Opcode::UNKNOWN_TYPE,   std::string("Unknown") },
        { Opcode::COMMAND,        std::string("Command") },
        { Opcode::INT_NUMBER,     std::string("I") },
        { Opcode::FLOAT_NUMBER,   std::string("I") },
        { Opcode::REGISTER,       std::string("R") },
        { Opcode::MEMORY,         std::string("M") },
        { Opcode::LABEL,          std::string("I") },
        { Opcode::NUMBER,         std::string("I") },
        { Opcode::CONDITION,      std::string("Condition") },
        { Opcode::FLAG,           std::string("F") },
        { Opcode::NAMED_REGISTER, std::string("NR") }
    };
    
    // Conditional suffix lookup.
    // Basically if the token that is expected to be the command is one of these
    // it is taken to be the conditional for that command and then the next token
    // is expected to be the command.
    std::map<std::string, Opcode::Conditionals> Opcode::s_conditional_suffix = {
        { "EQ", Opcode::COND_EQ },
        { "==", Opcode::COND_EQ },
        { "NE", Opcode::COND_NE },
        { "!=", Opcode::COND_NE },

        { "LT", Opcode::COND_LT },
        { "<",  Opcode::COND_LT },
        { "LE", Opcode::COND_LE },
        { "<=", Opcode::COND_LE },
        
        { "GT", Opcode::COND_GT },
        { ">",  Opcode::COND_GT },
        { "GE", Opcode::COND_GE },
        { ">=", Opcode::COND_GE }
    };

    // Names for flags.
    // If a token that is expected to be an argument is one of these strings
    // it is now recognised as a flag value. This means that labels
    // cannot be one of these values.
    std::map<std::string, Registers::Flags> Opcode::s_names_to_flags = {
        { "ZERO",     Registers::FLAG_ZERO },
        { "NEGATIVE", Registers::FLAG_NEGATIVE },
        { "CARRY",    Registers::FLAG_CARRY },
        { "OVERFLOW", Registers::FLAG_OVERFLOW },
        { "MMU",      Registers::FLAG_ENABLE_MMU }
    };

    std::map<std::string, Opcode::NamedRegister> Opcode::s_names_to_named_reg = {
        { "PROGRAM_COUNTER",    Opcode::PROGRAM_COUNTER },
        { "STACK_POINTER",      Opcode::STACK_POINTER },
        { "PAGE_TABLE_POINTER", Opcode::PAGE_TABLE_POINTER }
    };

}
