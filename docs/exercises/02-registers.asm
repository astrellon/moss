main:
    MOV r0 5      ; Can be read as register 0 = 5
    MOV r1 8      ; Can be read as register 1 = 8
    ADD r2 r0 r1  ; Can be read as register 2 = register 0 + register 1
    ADD r2 3      ; Can be read as register 2 += 3
    INFO r2       ; Debug command for displaying information about a register.
