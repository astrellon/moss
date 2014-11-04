#include "opcode.h"

#include <sstream>

namespace moss
{
    Opcode::Command Opcode::find_command(const std::string &command_name)
    {
        //return UNKNOWN_COMMAND;
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

    std::map<std::string, Opcode::Command> Opcode::s_names_to_commands = {
        { std::string("HALT"), Opcode::HALT },
        
        { std::string("MOV_R_R"), Opcode::MOV_R_R },
        { std::string("MOV_R_I"), Opcode::MOV_R_I },
        { std::string("MOV_M_R"), Opcode::MOV_M_R },
        { std::string("MOV_R_M"), Opcode::MOV_R_M },
        { std::string("MOV_M_I"), Opcode::MOV_M_I },
        { std::string("MOV_M_M"), Opcode::MOV_M_M },

        { std::string("UINT_FLOAT_R"),      Opcode::UINT_FLOAT_R },
        { std::string("UINT_FLOAT_R_R"),    Opcode::UINT_FLOAT_R_R },
        { std::string("FLOAT_UINT_R"),      Opcode::FLOAT_UINT_R },
        { std::string("FLOAT_UINT_R_R"),    Opcode::FLOAT_UINT_R_R },
        
        { std::string("ADD_R_R"),   Opcode::ADD_R_R },
        { std::string("ADD_R_R_R"), Opcode::ADD_R_R_R },
        { std::string("ADD_R_R_I"), Opcode::ADD_R_R_I },
        { std::string("ADD_R_I"),   Opcode::ADD_R_I },
        
        { std::string("ADDF_R_R"),   Opcode::ADDF_R_R },
        { std::string("ADDF_R_R_R"), Opcode::ADDF_R_R_R },
        { std::string("ADDF_R_R_I"), Opcode::ADDF_R_R_I },
        { std::string("ADDF_R_I"),   Opcode::ADDF_R_I },

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

        { std::string("PUSH_R"),    Opcode::PUSH_R },
        { std::string("PUSH_I"),    Opcode::PUSH_I },
        { std::string("POP_R"),     Opcode::POP_R },
        
        { std::string("CMP_R_R"),   Opcode::CMP_R_R },
        { std::string("CMP_R_I"),   Opcode::CMP_R_I },
        { std::string("CMP_I_R"),   Opcode::CMP_I_R },

        { std::string("CMPF_R_R"),  Opcode::CMPF_R_R },
        { std::string("CMPF_R_I"),  Opcode::CMPF_R_I },
        { std::string("CMPF_I_R"),  Opcode::CMPF_I_R },
        
        { std::string("JMP_R"),  Opcode::JMP_R },
        { std::string("JMP_I"),  Opcode::JMP_I },
        { std::string("JNE_R"),  Opcode::JNE_R },
        { std::string("JNE_I"),  Opcode::JNE_I },
        { std::string("JEQ_R"),  Opcode::JEQ_R },
        { std::string("JEQ_I"),  Opcode::JEQ_I },
        { std::string("JLT_R"),  Opcode::JLT_R },
        { std::string("JLT_I"),  Opcode::JLT_I },
        { std::string("JLE_R"),  Opcode::JLE_R },
        { std::string("JLE_I"),  Opcode::JLE_I },
        { std::string("JGT_R"),  Opcode::JGT_R },
        { std::string("JGT_I"),  Opcode::JGT_I },
        { std::string("JGE_R"),  Opcode::JGE_R },
        { std::string("JGE_I"),  Opcode::JGE_I },

        { std::string("PRINT_R"),  Opcode::PRINT_R }
    };

    std::map<Opcode::Command, std::pair<std::string, std::vector<Opcode::Type> > > Opcode::s_commands_to_types = {
        { Opcode::HALT, { "halt", {} } },

        { Opcode::MOV_R_R, { "mov", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::MOV_R_I, { "mov", { Opcode::REGISTER, Opcode::NUMBER } } },
        
        { Opcode::MOV_M_R, { "mov", { Opcode::MEMORY, Opcode::REGISTER } } },
        { Opcode::MOV_R_M, { "mov", { Opcode::REGISTER, Opcode::MEMORY } } },
        { Opcode::MOV_M_I, { "mov", { Opcode::MEMORY, Opcode::NUMBER } } },
        { Opcode::MOV_M_M, { "mov", { Opcode::MEMORY, Opcode::MEMORY } } },
        
        { Opcode::UINT_FLOAT_R, { "uint_float", { Opcode::REGISTER } } },
        { Opcode::UINT_FLOAT_R_R, { "uint_float", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::FLOAT_UINT_R, { "float_uint", { Opcode::REGISTER } } },
        { Opcode::FLOAT_UINT_R_R, { "float_uint", { Opcode::REGISTER, Opcode::REGISTER } } },
        
        { Opcode::ADD_R_R, { "add", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::ADD_R_R_R, { "add", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::ADD_R_R_I, { "add", { Opcode::REGISTER, Opcode::REGISTER, Opcode::INT_NUMBER } } },
        { Opcode::ADD_R_I, { "add", { Opcode::REGISTER, Opcode::INT_NUMBER } } },
        
        { Opcode::ADDF_R_R, { "addf", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::ADDF_R_R_R, { "addf", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::ADDF_R_R_I, { "addf", { Opcode::REGISTER, Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        { Opcode::ADDF_R_I, { "addf", { Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        
        { Opcode::SUB_R_R, { "sub", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SUB_R_R_R, { "sub", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SUB_R_I_R, { "sub", { Opcode::REGISTER, Opcode::INT_NUMBER, Opcode::REGISTER } } },
        { Opcode::SUB_R_R_I, { "sub", { Opcode::REGISTER, Opcode::REGISTER, Opcode::INT_NUMBER } } },
        { Opcode::SUB_R_I, { "sub", { Opcode::REGISTER, Opcode::INT_NUMBER } } },
        
        { Opcode::SUBF_R_R, { "subf", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SUBF_R_R_R, { "subf", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SUBF_R_I_R, { "subf", { Opcode::FLOAT_NUMBER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SUBF_R_R_I, { "subf", { Opcode::REGISTER, Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        { Opcode::SUBF_R_I, { "subf", { Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        
        { Opcode::PUSH_R, { "push", { Opcode::REGISTER } } },
        { Opcode::PUSH_I, { "push", { Opcode::NUMBER } } },
        { Opcode::POP_R, { "pop", { Opcode::REGISTER } } },
        
        { Opcode::CMP_R_R, { "cmp", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::CMP_R_I, { "cmp", { Opcode::REGISTER, Opcode::INT_NUMBER } } }, 
        { Opcode::CMP_I_R, { "cmp", { Opcode::INT_NUMBER, Opcode::REGISTER } } },
        
        { Opcode::CMPF_R_R, { "cmpf", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::CMPF_R_I, { "cmpf", { Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        { Opcode::CMPF_I_R, { "cmpf", { Opcode::FLOAT_NUMBER, Opcode::REGISTER } } },
        
        { Opcode::JMP_R, { "jmp", { Opcode::REGISTER } } },
        { Opcode::JMP_I, { "jmp", { Opcode::INT_NUMBER } } },
        { Opcode::JNE_R, { "jne", { Opcode::REGISTER } } },
        { Opcode::JNE_I, { "jne", { Opcode::INT_NUMBER } } },
        { Opcode::JEQ_R, { "jeq", { Opcode::REGISTER } } },
        { Opcode::JEQ_I, { "jeq", { Opcode::INT_NUMBER } } },
        { Opcode::JLT_R, { "jlt", { Opcode::REGISTER } } },
        { Opcode::JLT_I, { "jlt", { Opcode::INT_NUMBER } } },
        { Opcode::JLE_R, { "jle", { Opcode::REGISTER } } },
        { Opcode::JLE_I, { "jle", { Opcode::INT_NUMBER } } },
        { Opcode::JGT_R, { "jgt", { Opcode::REGISTER } } },
        { Opcode::JGT_I, { "jgt", { Opcode::INT_NUMBER } } },
        { Opcode::JGE_R, { "jge", { Opcode::REGISTER } } },
        { Opcode::JGE_I, { "jge", { Opcode::INT_NUMBER } } },
        
        { Opcode::PRINT_R, { "print", { Opcode::INT_NUMBER } } }
    };
    
    std::map<Opcode::Type, std::string> Opcode::s_type_names = {
        { Opcode::UNKNOWN_TYPE, std::string("unknown") },
        { Opcode::COMMAND, std::string("command") },
        { Opcode::INT_NUMBER, std::string("int_number") },
        { Opcode::FLOAT_NUMBER, std::string("float_number") },
        { Opcode::REGISTER, std::string("register") },
        { Opcode::MEMORY, std::string("memory") },
        { Opcode::LABEL, std::string("label") },
        { Opcode::NUMBER, std::string("number") }
    };
    
    std::map<Opcode::Type, std::string> Opcode::s_type_codes = {
        { Opcode::UNKNOWN_TYPE, std::string("Unknown") },
        { Opcode::COMMAND, std::string("Command") },
        { Opcode::INT_NUMBER, std::string("I") },
        { Opcode::FLOAT_NUMBER, std::string("I") },
        { Opcode::REGISTER, std::string("R") },
        { Opcode::MEMORY, std::string("M") },
        { Opcode::LABEL, std::string("I") },
        { Opcode::NUMBER, std::string("I") }
    };
}
