; Second test tile

main:
    MOV r0 0
start:
    CMP r0 0x00FFFFFF
    >= JMP end
    INC r0
    JMP start
end:
    HALT
