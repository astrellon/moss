; Third test tile
    MOV r0 0
    SEND 0 15
start:
    ;CMP r0 0x00FFFFFF
    CMP r0 10
    < INC r0
    < JMP start
end:
    HALT
