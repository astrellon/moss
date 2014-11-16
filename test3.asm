; Third test tile

; MMU setup
; Sets a 1 to 1 mapping up to 32
    MOV r0 0    ; Page table index
mmu_start:
    CMP r0 32
    < MOV @r0 r0
    INC r0
    < JMP mmu_start


; Peripheral setup
    MOV r0 0    ; Perf index
    MOV r4 64   ; Perf memory offset
perf_start:
    ; Send general port status, returns:
    ;  1: Attached and assigned
    ;  0: Attached but not assigned
    ; -1: Nothing attached
    ; -2: Out of bounds for peripherals
    ; -3: Other error
    IO_SEND r1 r0 0
    CMP r1 0
    == JMP perf_assign  ; Unassigned
    
    INC r0
    > JMP perf_start    ; Assigned
    
    CMP r1 -1           ; Check if attached
    == JMP perf_start   ; Not attached, skip
    < JMP perf_end      ; At the end of the perfs

perf_assign:
    IO_SEND r1 r0 1     ; Ask the peripheral how much memory it wants.
    IO_ASSIGN r0 r4 r1
    ADD r4 r1
    INC r0
    JMP perf_start

perf_end:


; Main program
    MOV r1 0
start:
    ;CMP r0 0x00FFFFFF
    CMP r1 10
    < INC r1
    < JMP start
end:
    PRINT "Finishing"
