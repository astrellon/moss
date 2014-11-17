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
    MOV r5 32   ; Perf memory data (offset, size)
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
    PRINT "Assinging perf"
    IO_SEND r1 r0 1     ; Ask the peripheral how much memory it wants.
    
    MOV @r5 r4          ; Store the memory offset of the perf
    INC r5
    MOV @r5 r1          ; Store the amount of memory the perf wanted.
    INC r5
    
    IO_ASSIGN r0 r4 r1
    ADD r4 r1
    INC r0
    JMP perf_start

perf_end:


; Main program
    MOV r5 32
    MOV r0 @r5          ; Grab the offset of the first perf
    INC r5
    MOV r1 @r5          ; Grab the size of the first perf

    MOV r2 0
    MOV r3 5
mem_start:
    CMP r2 r1
    >= JMP mem_end
    MOV @r0 r3
    ADD r3 5
    INC r2
    JMP mem_start

mem_end:
    IO_SEND 0 0x12
    ;CMP r0 0x00FFFFFF
    ;CMP r1 10
    ;< INC r1
    ;< JMP start

end:
    PRINT "Finishing"
