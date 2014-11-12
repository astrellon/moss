; Third test tile
    MOV r0 0
    CMP r0 0
    ;MOV ZERO 0
    == MOV r8 123
    != MOV r8 456
start:
    ;CMP r0 0x00FFFFFF
    CMP r0 10
    < INC r0
    < JMP start
end:
    HALT
