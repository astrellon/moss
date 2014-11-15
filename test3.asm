; Third test tile
    MOV r0 0
    MOV r1 0
mmu_start:
    CMP r0 32
    < MOV @r0 r0
    INC r0
    < JMP mmu_start

    ;IO_SEND r2 0 10
    ;IO_SEND r3 0 14
    ;IO_ASSIGN 0 64 32
start:
    ;CMP r0 0x00FFFFFF
    CMP r1 10
    < INC r1
    < JMP start
end:
    HALT
