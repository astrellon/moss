; STD Math functions
func_add:
    POP r7
    POP r8
    ADD r7 r8

    PUSH r7
    RETURN

func_double:
    POP r7

    MUL r7 3

    PUSH r7
    RETI
