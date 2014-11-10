; Third test tile
    MOV r0 0
start:
    CMP r0 0x00FFFFFF
    < INC r0
    < JMP start
end:
    HALT
