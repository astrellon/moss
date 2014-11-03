; Test file
    MOVF fr0 34.5
    MOV r0 0
    MOV r1 32
start:
    CMP r0 32
    JGE end
    MOV r2 @r1
    ADD r0 1
    ADD r1 1
    JMP start
end:
    UINT_FLOAT fr1 r2
    ADDF fr2 fr1 fr0
    FLOAT_UINT r5 fr2
    MOV r6 1337


