; Third test tile
    MOV r0 0
    SEND r2 0 10
    SEND r3 0 14
start:
    ;CMP r0 0x00FFFFFF
    CMP r0 10
    < INC r0
    < JMP start
end:
    HALT
