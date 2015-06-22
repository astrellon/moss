; Setup names for registers.
#define number_of_dice r3
#define number_of_sides r4
#define rand_seed r10
#define max_uint r5

main:
    ; Seed random number generator
    TIME rand_seed
    
    ; Max uint value
    MOV max_uint 0xFFFFFFFF
    ; Max uint value as a float
    UINT_FLOAT max_uint

    PRINT "Enter number of dice: "
    INPUT number_of_dice

    PRINT "Enter number of sides: "
    INPUTF number_of_sides

    MOV r0 0
start_loop:
    CMP r0 number_of_dice
    >= JMP end

    ; Call function
    PUSH number_of_sides
    CALL roll_dice
    POP r1

    PRINT "Dice roll: "
    PRINT r1
    PRINT "\n"

    INC r0
    JMP start_loop

end:
    PRINT "Done\n"
    HALT


; Roll dice for N number of sides
roll_dice:
    ; Get number of sides
    POP r2

    ; Get random value
    CALL rand
    POP r1

    ; Multiply by the number of sides
    MULF r1 r2

    ; Floor sides by converting to int
    FLOAT_UINT r1

    ; Add one
    INC r1
    
    ; Return value
    PUSH r1
    RETURN

; Function for creating random numbers
rand:
    MUL rand_seed 1103515245
    ADD rand_seed 12345

    UINT_FLOAT rand_seed
    DIVF rand_seed max_uint
    
    PUSH rand_seed
    RETURN

