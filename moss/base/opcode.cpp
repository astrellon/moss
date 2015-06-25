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
        return named_register_name(static_cast<uint32_t>(flag));
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

    // Names to commands {{{
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
		{ std::string("INT_FLOAT_R"),   Opcode::INT_FLOAT_R },
		{ std::string("INT_FLOAT_R_R"), Opcode::INT_FLOAT_R_R },
		{ std::string("FLOAT_INT_R"),   Opcode::FLOAT_INT_R },
		{ std::string("FLOAT_INT_R_R"), Opcode::FLOAT_INT_R_R },
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
		{ std::string("ADD_R_I_R"), Opcode::ADD_R_I_R },
		{ std::string("ADD_R_R_I"), Opcode::ADD_R_R_I },
		{ std::string("ADD_R_I"),   Opcode::ADD_R_I },

		{ std::string("ADDF_R_R"),   Opcode::ADDF_R_R },
		{ std::string("ADDF_R_R_R"), Opcode::ADDF_R_R_R },
		{ std::string("ADDF_R_I_R"), Opcode::ADDF_R_I_R },
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
		{ std::string("MUL_R_I_R"), Opcode::MUL_R_I_R },
		{ std::string("MUL_R_R_I"), Opcode::MUL_R_R_I },
		{ std::string("MUL_R_I"),   Opcode::MUL_R_I },

		{ std::string("MULF_R_R"),   Opcode::MULF_R_R },
		{ std::string("MULF_R_R_R"), Opcode::MULF_R_R_R },
		{ std::string("MULF_R_I_R"), Opcode::MULF_R_I_R },
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
        
        // Function commands {{{
        { std::string("CALL_I"),  Opcode::CALL_I },
        { std::string("CALL_R"),  Opcode::CALL_R },
        { std::string("RETURN"),  Opcode::RETURN },
        // }}}
        
        // Interrupt commands {{{
        { std::string("REGI_I_I"), Opcode::REGI_I_I },
        { std::string("REGI_I_R"), Opcode::REGI_I_R },
        { std::string("REGI_R_I"), Opcode::REGI_R_I },
        { std::string("REGI_R_R"), Opcode::REGI_R_R },

        { std::string("INT_I"), Opcode::INT_I },
        { std::string("INT_R"), Opcode::INT_R },
        { std::string("RETI"),  Opcode::RETI },
        // }}}
        
        { std::string("PRINT_R"),  Opcode::PRINT_R },
        { std::string("PRINT_I"),  Opcode::PRINT_I },
        { std::string("PRINT_S"),  Opcode::PRINT_S },
        
        { std::string("PRINTF_R"),  Opcode::PRINTF_R },
        { std::string("PRINTF_I"),  Opcode::PRINTF_I },

        { std::string("INFO_R"), Opcode::INFO_R },
        
        { std::string("INPUT_R"),  Opcode::INPUT_R },
        { std::string("INPUTF_R"),  Opcode::INPUTF_R },
        
        { std::string("TIME_R"),  Opcode::TIME_R },
    };
    // }}}

    // Commands to types {{{
    // This map is used by the disassembler for converting opcode values back
    // into the original command name with the expected arguments. 
    std::map<Opcode::Command, std::pair<std::string, std::vector<Opcode::Type> > > Opcode::s_commands_to_types = {
        // Halt
        /**
         * Stops the CPU from running completely.
         */
        { Opcode::HALT, { "HALT", {} } },

        // MOV {{{
        /** 
         * The MOV command is used to move data around between registers and memory.
         * It can also be used to set registers and memory to a hardcoded value.
         *
         * <div>For example:
         * <pre>MOV r0 5    ; Sets register 0 to 5.
         * MOV r1 r0        ; Sets reigster 1 to register 0</pre>
         * </div>
         */
        { Opcode::MOV_R_R, { "MOV", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::MOV_R_I, { "MOV", { Opcode::REGISTER, Opcode::NUMBER } } },
        { Opcode::MOV_M_R, { "MOV", { Opcode::MEMORY, Opcode::REGISTER } } },
       
        { Opcode::MOV_R_M, { "MOV", { Opcode::REGISTER, Opcode::MEMORY } } },
        { Opcode::MOV_M_I, { "MOV", { Opcode::MEMORY, Opcode::NUMBER } } },
        { Opcode::MOV_M_M, { "MOV", { Opcode::MEMORY, Opcode::MEMORY } } },
        
        { Opcode::MOV_R_F, { "MOV", { Opcode::REGISTER, Opcode::FLAG } } },
        { Opcode::MOV_F_R, { "MOV", { Opcode::FLAG, Opcode::REGISTER } } },
        { Opcode::MOV_F_I, { "MOV", { Opcode::FLAG, Opcode::INT_NUMBER } } },
        
        { Opcode::MOV_R_NR, { "MOV", { Opcode::REGISTER, Opcode::NAMED_REGISTER } } },
        { Opcode::MOV_NR_R, { "MOV", { Opcode::NAMED_REGISTER, Opcode::REGISTER } } },
        { Opcode::MOV_NR_I, { "MOV", { Opcode::NAMED_REGISTER, Opcode::NUMBER } } },
        // }}}
        
        // Unit conversion {{{
        /**
         * Converts a int register value into a float value.
         * Either storing the result in the same register or in another register.
         *
         * <div>For example:
         * <pre>MOV r0 6
         * INFO r0          ; The value in register 0 is gibberish as a float.
         * INT_FLOAT r0     ; Convert the integer to a float
         * INFO r0          ; The value in register 0 is now what you'd expect it to be.</pre>
         * </div>
         */
        { Opcode::INT_FLOAT_R,   { "INT_FLOAT", { Opcode::REGISTER } } },
        { Opcode::INT_FLOAT_R_R, { "INT_FLOAT", { Opcode::REGISTER, Opcode::REGISTER } } },
        /**
         * Converts a float register value into a int value.
         * Either storing the result in the same register or in another register.
         * <div>This can be used as a way to round down a float.</div>
         *
         * <div>For example:
         * <pre>MOV r0 4.5
         * INFO r0          ; The value in register 0 is gibberish as an integer.
         * FLOAT_INT r0     ; Convert the float to a integer
         * INFO r0          ; The value in register 0 is now 4.</pre>
         * </div>
         */
        { Opcode::FLOAT_INT_R,   { "FLOAT_INT", { Opcode::REGISTER } } },
        { Opcode::FLOAT_INT_R_R, { "FLOAT_INT", { Opcode::REGISTER, Opcode::REGISTER } } },
        // }}}
        
        // Stack {{{
        /**
         * Pushes either a number or a register value onto the stack.
         * It doesn't matter what kind of value it is.
         *
         * <div>For example:
         * <pre>MOV r0 8
         * PUSH r0      ; There is now one item on the stack.
         * PUSH 4       ; Now there are two items on the stack
         * POP r1       ; Take the top most item and put it into register 1
         * POP r2       ; Take the next item and put it into register 2
         * &nbsp;            ; The stack is now empty at this point.
         *
         * INFO r1      ; Prints 4
         * INFO r2      ; Prints 8</pre>
         * </div>
         */
        { Opcode::PUSH_R, { "PUSH", { Opcode::REGISTER } } },
        { Opcode::PUSH_I, { "PUSH", { Opcode::NUMBER } } },
        /**
         * Pops the top most value off the stack into a register.
         * <div>See PUSH command</div>
         */
        { Opcode::POP_R,  { "POP",  { Opcode::REGISTER } } },
        // }}}
        
        // CMP/CMPF {{{
        /**
         * Compare operation. Once this has been performed between a register and/or
         * an integer number, conditional operators can be used to either execute or 
         * ignore subsequent commands.
         *
         * <div>
         * For example:
         * <pre>MOV r0 5
         * CMP r0 10
         * > INC r0     ; Won't run because 5 is not greater than 10
         * < DEC r0     ; Will run because 5 is less than 10
         * == INC r0    ; Won't run because 5 does not equal 10
         * != DEC r0    ; Will run because 5 does not equal 10
         *
         * INFO r0      ; Will output 3 because the two DEC commands executed.</pre>
         * </div>
         */
        { Opcode::CMP_R_R, { "CMP", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::CMP_R_I, { "CMP", { Opcode::REGISTER, Opcode::INT_NUMBER } } }, 
        { Opcode::CMP_I_R, { "CMP", { Opcode::INT_NUMBER, Opcode::REGISTER } } },
        
        /**
         * Compare operation. Once this has been performed between a register and/or
         * a float number, conditional operators can be used to either execute or
         * ignore subsequent commands.
         *
         * <div>
         * For example:
         * <pre>MOV r0 5.5
         * CMPF r0 10.2
         * > ADDF r0 1.5    ; Won't run because 5.5 is not greater than 10.2
         * < SUBF r0 1.5    ; Will run because 5.5 is less than 10.2
         * == ADDF r0 1.5   ; Won't run because 5.5 does not equal 10.2
         * != SUBF r0 1.5   ; Will run because 5.5 does not equal 10.2
         *
         * INFO r0          ; Will output 2.5 because the two SUBF commands executed.</pre>
         * </div>
         */
        { Opcode::CMPF_R_R, { "CMPF", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::CMPF_R_I, { "CMPF", { Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        { Opcode::CMPF_I_R, { "CMPF", { Opcode::FLOAT_NUMBER, Opcode::REGISTER } } },
        // }}}
        
        // Branching {{{
        /**
         * Changes the location of the instruction pointer.
         * Usually used to jump to a label, but can be used to just to any number.
         *
         * For example:
         * <pre>MOV r0 5
         * JMP skip     ; Jumps to the skip label
         * MOV r0 10    ; Will be jumped over
         * skip:
         * INFO r0      ; Will print 5 </pre>
         */
        { Opcode::JMP_R, { "JMP", { Opcode::REGISTER } } },
        { Opcode::JMP_I, { "JMP", { Opcode::INT_NUMBER } } },
        // }}}
        
        // ADD/ADDF {{{
        /**
         * Add two integer values together.
         * Either storing the result in the first register, or storing the result in 
         * another register.
         *
         * <div>For example:
         * <pre>MOV r0 6
         * ADD r0 4
         * INFO r0      ; Prints 10, as 4 has been added to the register.
         * ADD r1 r0 6  
         * INFO r0      ; Still prints 10 as the result of the add wasn't put back into the same register.
         * INFO r1      ; Prints 16 as the result was stored into a different register.</pre>
         * </div>
         */
        { Opcode::ADD_R_R,    { "ADD", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::ADD_R_R_R,  { "ADD", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::ADD_R_I_R,  { "ADD", { Opcode::REGISTER, Opcode::INT_NUMBER, Opcode::REGISTER } } },
        { Opcode::ADD_R_R_I,  { "ADD", { Opcode::REGISTER, Opcode::REGISTER, Opcode::INT_NUMBER } } },
        { Opcode::ADD_R_I,    { "ADD", { Opcode::REGISTER, Opcode::INT_NUMBER } } },
        
        /**
         * Add two floats values together.
         * Either storing the result in the first register, or storing the result in 
         * another register.
         *
         * <div>For example:
         * <pre>MOV r0 7.6
         * ADDF r0 3.4
         * INFO r0      ; Prints 10.1, as the two values have been added together and stored back into register 0.
         * ADDF r1 r0 7.5
         * INFO r0      ; Prints 10.1
         * INFO r1      ; Prints 17.6
         * </div>
         */
        { Opcode::ADDF_R_R,   { "ADDF", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::ADDF_R_R_R, { "ADDF", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::ADDF_R_I_R, { "ADDF", { Opcode::REGISTER, Opcode::FLOAT_NUMBER, Opcode::REGISTER } } },
        { Opcode::ADDF_R_R_I, { "ADDF", { Opcode::REGISTER, Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        { Opcode::ADDF_R_I,   { "ADDF", { Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        // }}}
        
        // SUB/SUBF {{{
        /**
         * Subtract two ints from each other.
         * Either storing the result into the first register, or storing the result in another register.
         *
         * <div>For example:
         * <pre>MOV r0 8
         * SUB r0 5
         * INFO r0      ; Prints 3
         * SUB r1 r0 10
         * INFO r0      ; Prints 3
         * INFO r1      ; Prints -7</pre>
         * </div>
         */
        { Opcode::SUB_R_R,    { "SUB", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SUB_R_R_R,  { "SUB", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SUB_R_I_R,  { "SUB", { Opcode::REGISTER, Opcode::INT_NUMBER, Opcode::REGISTER } } },
        { Opcode::SUB_R_R_I,  { "SUB", { Opcode::REGISTER, Opcode::REGISTER, Opcode::INT_NUMBER } } },
        { Opcode::SUB_R_I,    { "SUB", { Opcode::REGISTER, Opcode::INT_NUMBER } } },
        
        /**
         * Subtract two floats from each other.
         * Either storing the result into the first register, or storing the result in another register.
         *
         * <div>For example:
         * <pre>MOV r0 8.7
         * SUBF r0 3.3
         * INFO r0      ; Prints 5.3
         * SUBF r1 r0 7.4
         * INFO r0      ; Prints 5.3
         * INFO r1      ; Prints -2</pre>
         * </div>
         */
        { Opcode::SUBF_R_R,   { "SUBF", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SUBF_R_R_R, { "SUBF", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SUBF_R_I_R, { "SUBF", { Opcode::FLOAT_NUMBER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::SUBF_R_R_I, { "SUBF", { Opcode::REGISTER, Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        { Opcode::SUBF_R_I,   { "SUBF", { Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        // }}}

        // INC/DEC {{{
        /**
         * Increment an integer register by one.
         *
         * <div>For example:
         * <pre>MOV r0 9
         * INC r0
         * INFO r0      ; Prints 10</pre>
         * </div>
         */
        { Opcode::INC_R,  { "INC",  { Opcode::REGISTER } } },
        /**
         * Increment a float register by one.
         *
         * <div>For example:
         * <pre>MOV r0 5.4
         * INCF r0
         * INFO r0      ; Prints 6.4</pre>
         * </div>
         */
        { Opcode::INCF_R, { "INCF", { Opcode::REGISTER } } },
        /**
         * Decrements an integer register by one.
         *
         * <div>For example:
         * <pre>
         * MOV r0 4
         * DEC r0
         * INFO r0      ; Prints 3</pre>
         * </div>
         */
        { Opcode::DEC_R,  { "DEC",  { Opcode::REGISTER } } },
        /**
         * Decrements a float register by one.
         *
         * <div>For example:
         * <pre>MOV r0 2.3
         * DECF r0
         * INFO r0      ; Prints 1.3</pre>
         * </div>
         */
        { Opcode::DECF_R, { "DECF", { Opcode::REGISTER } } },
        // }}}
        
        // MUL/MULF {{{
        /**
         * Multiply two integers together.
         *
         * <div>For example:
         * <pre>MOV r0 4
         * MUL r0 5
         * INFO r0      ; Prints 20
         * MUL r1 r0 2
         * INFO r0      ; Prints 20
         * INFO r1      ; Prints 40</pre>
         * </div>
         */
        { Opcode::MUL_R_R,    { "MUL", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::MUL_R_R_R,  { "MUL", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::MUL_R_I_R,  { "MUL", { Opcode::REGISTER, Opcode::INT_NUMBER, Opcode::REGISTER } } },
        { Opcode::MUL_R_R_I,  { "MUL", { Opcode::REGISTER, Opcode::REGISTER, Opcode::INT_NUMBER } } },
        { Opcode::MUL_R_I,    { "MUL", { Opcode::REGISTER, Opcode::INT_NUMBER } } },
        
        /**
         * Multiply two floats together.
         * 
         * <div>For example:
         * <pre>MOV r0 10.0
         * MULF r0 2.5
         * INFO r0      ; Prints 25.0
         * MULF r1 r0 0.5
         * INFO r0      ; Prints 25.0
         * INFO r1      ; Prints 12.5</pre>
         * </div>
         */
        { Opcode::MULF_R_R,   { "MULF", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::MULF_R_R_R, { "MULF", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::MULF_R_I_R, { "MULF", { Opcode::REGISTER, Opcode::FLOAT_NUMBER, Opcode::REGISTER } } },
        { Opcode::MULF_R_R_I, { "MULF", { Opcode::REGISTER, Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        { Opcode::MULF_R_I,   { "MULF", { Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        // }}}
        
        // DIV/DIVF {{{
        /**
         * Divide two integers.
         *
         * <div>For example:
         * <pre>MOV r0 8
         * DIV r0 2
         * INFO r0      ; Prints 4
         * DIV r1 r0 3  
         * INFO r0      ; Prints 4
         * INFO r1      ; Prints 1, due to integer rounding.</pre>
         * </div>
         */
        { Opcode::DIV_R_R,    { "DIV", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::DIV_R_R_R,  { "DIV", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::DIV_R_I_R,  { "DIV", { Opcode::REGISTER, Opcode::INT_NUMBER, Opcode::REGISTER } } },
        { Opcode::DIV_R_R_I,  { "DIV", { Opcode::REGISTER, Opcode::REGISTER, Opcode::INT_NUMBER } } },
        { Opcode::DIV_R_I,    { "DIV", { Opcode::REGISTER, Opcode::INT_NUMBER } } },
        
        /**
         * Divide two floats.
         *
         * <div>For example:
         * <pre>MOV r0 6.6
         * DIVF r0 2.0
         * INFO r0      ; Prints 3.3
         * DIVF r1 r0 3.0
         * INFO r0      ; Prints 3.3
         * INFO r1      ; Prints 1.1</pre>
         * </div>
         */
        { Opcode::DIVF_R_R,   { "DIVF", { Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::DIVF_R_R_R, { "DIVF", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::DIVF_R_I_R, { "DIVF", { Opcode::FLOAT_NUMBER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::DIVF_R_R_I, { "DIVF", { Opcode::REGISTER, Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        { Opcode::DIVF_R_I,   { "DIVF", { Opcode::REGISTER, Opcode::FLOAT_NUMBER } } },
        // }}}
        
        // ROR/ROL {{{
        /**
         * Bit shift rotate right.
         * Rotate shiting basically pushes all the bits in a number one over to the right. If the right most
         * bit is 'pushed off' the edge it comes back to the start of the number. So if you were to rotate 
         * right 32 times you'd come back to the same number you started off with.
         *
         * <div>For example:
         * <pre>MOV r0 0x10020005    ; In binary this should be 0001 0000 0000 0010 0000 0000 0101 or 268435459 in decimal.
         * ROR r0
         * INFO r0          ; Prints 2281766914 decimal, which is 1000 1000 0000 0001 0000 0000 0010 in binary.
         * ROR r1 r0        
         * INFO r1          ; Prints 1140883457 decimal, which is 0100 0100 0000 0000 1000 0000 0001 in binary.
         * INFO r0          ; Prints 2281766914.<pre>
         * </div>
         */
        { Opcode::ROR_R,   { "ROR", { Opcode::REGISTER } } },
        { Opcode::ROR_R_R, { "ROR", { Opcode::REGISTER, Opcode::REGISTER } } },
        
        /**
         * Bit shift rotate left.
         * <pre>MOV r0 0xC0000000    ; In binary this should be 1100 0000 0000 0000
         * ROR r0
         */
        { Opcode::ROL_R,   { "ROL", { Opcode::REGISTER } } },
        { Opcode::ROL_R_R, { "ROL", { Opcode::REGISTER, Opcode::REGISTER } } },
        // }}}
        
        // SHR/SHL {{{
        /**
         * Bit shift right.
         */
        { Opcode::SHR_R,   { "SHR", { Opcode::REGISTER } } },
        { Opcode::SHR_R_R, { "SHR", { Opcode::REGISTER, Opcode::REGISTER } } },
        
        /**
         * Bit shift left.
         */
        { Opcode::SHL_R,   { "SHL", { Opcode::REGISTER } } },
        { Opcode::SHL_R_R, { "SHL", { Opcode::REGISTER, Opcode::REGISTER } } },
        // }}}

        // Peripherals {{{

        // SEND {{{
        /**
         * Send data to a port.
         * Alternatively send data to a port and save the response to a register.
         */
        { Opcode::IO_SEND_I_I, { "IO_SEND", { Opcode::INT_NUMBER, Opcode::NUMBER } } },
        { Opcode::IO_SEND_R_I, { "IO_SEND", { Opcode::REGISTER, Opcode::NUMBER } } },
        { Opcode::IO_SEND_I_R, { "IO_SEND", { Opcode::INT_NUMBER, Opcode::REGISTER } } },
        { Opcode::IO_SEND_R_R, { "IO_SEND", { Opcode::REGISTER, Opcode::REGISTER } } },
        
        { Opcode::IO_SEND_R_I_I, { "IO_SEND", { Opcode::REGISTER, Opcode::INT_NUMBER, Opcode::NUMBER } } },
        { Opcode::IO_SEND_R_R_I, { "IO_SEND", { Opcode::REGISTER, Opcode::REGISTER, Opcode::NUMBER } } },
        { Opcode::IO_SEND_R_I_R, { "IO_SEND", { Opcode::REGISTER, Opcode::INT_NUMBER, Opcode::REGISTER } } },
        { Opcode::IO_SEND_R_R_R, { "IO_SEND", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        // }}}
        
        // ASSIGN {{{
        /**
         * Assigning memory to a port.
         */
        { Opcode::IO_ASSIGN_R_R_R, { "IO_ASSIGN", { Opcode::REGISTER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::IO_ASSIGN_R_R_I, { "IO_ASSIGN", { Opcode::REGISTER, Opcode::REGISTER, Opcode::NUMBER } } },
        { Opcode::IO_ASSIGN_R_I_R, { "IO_ASSIGN", { Opcode::REGISTER, Opcode::NUMBER, Opcode::REGISTER } } },
        { Opcode::IO_ASSIGN_R_I_I, { "IO_ASSIGN", { Opcode::REGISTER, Opcode::NUMBER, Opcode::NUMBER } } },
        
        { Opcode::IO_ASSIGN_I_R_R, { "IO_ASSIGN", { Opcode::NUMBER, Opcode::REGISTER, Opcode::REGISTER } } },
        { Opcode::IO_ASSIGN_I_R_I, { "IO_ASSIGN", { Opcode::NUMBER, Opcode::REGISTER, Opcode::NUMBER } } },
        { Opcode::IO_ASSIGN_I_I_R, { "IO_ASSIGN", { Opcode::NUMBER, Opcode::NUMBER, Opcode::REGISTER } } },
        { Opcode::IO_ASSIGN_I_I_I, { "IO_ASSIGN", { Opcode::NUMBER, Opcode::NUMBER, Opcode::NUMBER } } },
        // }}}
        
        // }}}

        // Function commands {{{
        /**
         * Function call
         */
        { Opcode::CALL_I, { "CALL", { Opcode::INT_NUMBER } } },
        { Opcode::CALL_R, { "CALL", { Opcode::REGISTER } } },

        /**
         * Function return
         */
        { Opcode::RETURN, { "RETURN", { } } },
        // }}}
        
        // Interrupt commands {{{
        /**
         * Register interrupt
         */
        { Opcode::REGI_I_I, { "REGI", { Opcode::INT_NUMBER, Opcode::INT_NUMBER } } },
        { Opcode::REGI_I_R, { "REGI", { Opcode::INT_NUMBER, Opcode::REGISTER } } },
        { Opcode::REGI_R_I, { "REGI", { Opcode::REGISTER, Opcode::INT_NUMBER } } },
        { Opcode::REGI_R_R, { "REGI", { Opcode::REGISTER, Opcode::REGISTER } } },

        /**
         * Trigger int
         */
        { Opcode::INT_I, { "INT", { Opcode::INT_NUMBER } } },
        { Opcode::INT_R, { "INT", { Opcode::REGISTER } } },

        /**
         * Return from interrupt.
         */
        { Opcode::RETI,  { "RETI", { } } },
        // }}}
        
        // Debugging commands {{{
        /**
         * Print ints and strings command
         */
        { Opcode::PRINT_R, { "PRINT", { Opcode::REGISTER } } },
        { Opcode::PRINT_I, { "PRINT", { Opcode::INT_NUMBER } } },
        { Opcode::PRINT_S, { "PRINT", { Opcode::STRING } } },
        
        /**
         * Print floats
         */
        { Opcode::PRINTF_R, { "PRINTF", { Opcode::REGISTER } } },
        { Opcode::PRINTF_I, { "PRINTF", { Opcode::FLOAT_NUMBER } } },
        
        /**
         * Displays the register number, its value as an integer and as a float.
         */
        { Opcode::INFO_R, { "INFO", { Opcode::REGISTER } } },

        /**
         * Get integer from keyboard.
         */
        { Opcode::INPUT_R, { "INPUT", { Opcode::REGISTER } } },
        /**
         * Get float from keyboard.
         */
        { Opcode::INPUTF_R, { "INPUTF", { Opcode::REGISTER } } },
        
        /**
         * Get the current time as a uint and store it in a register.
         */
        { Opcode::TIME_R, { "TIME", { Opcode::REGISTER } } },
        // }}}
    };
    // }}}
    
    // Converts opcode types to strings. Mostly for debugging the token parsing output. {{{
    std::map<Opcode::Type, std::string> Opcode::s_type_names = {
        { Opcode::UNKNOWN_TYPE,   std::string("unknown") },
        { Opcode::COMMAND,        std::string("command") },
        { Opcode::INT_NUMBER,     std::string("int_number") },
        { Opcode::FLOAT_NUMBER,   std::string("float_number") },
        { Opcode::REGISTER,       std::string("register") },
        { Opcode::MEMORY,         std::string("memory") },
        { Opcode::LABEL,          std::string("label") },
        { Opcode::NUMBER,         std::string("number") },
        { Opcode::CONDITION,      std::string("condition") },
        { Opcode::FLAG,           std::string("flag") },
        { Opcode::NAMED_REGISTER, std::string("named_register") },
        { Opcode::STRING,         std::string("string") }
    };
    // }}}
    
    // For creating the command lookup key. {{{
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
        { Opcode::NAMED_REGISTER, std::string("NR") },
        { Opcode::STRING,         std::string("S") }
    };
    // }}}
    
    // Conditional suffix lookup. {{{
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
    // }}}

    // Names for named registers. {{{
    std::map<std::string, Opcode::NamedRegister> Opcode::s_names_to_named_reg = {
        { "PROGRAM_COUNTER",    Opcode::PROGRAM_COUNTER },
        { "STACK_POINTER",      Opcode::STACK_POINTER },
        { "CODE_STACK_POINTER",      Opcode::CODE_STACK_POINTER },
        { "PAGE_TABLE_POINTER", Opcode::PAGE_TABLE_POINTER }
    };
    // }}}

}
