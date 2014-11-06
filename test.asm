; Test file
    MOV r11 0x80000000
    ROR r12 r11
    ROL r13 r11
    MOV r8 34.5
    MOV r15 7.23f
    MOV r0 0
    MOV r1 0x20
start:
    CMP r0 32
    JGE end
    MOV r2 @r1
    INC r0
    INC r1
    JMP start
end:
    UINT_FLOAT r9 r2
    ADDF r10 r8 r9
    FLOAT_UINT r5 r10
    MOV r6 1337
    HALT

