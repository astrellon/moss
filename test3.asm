; Third test file

#include "stdmath.asm"

; MMU setup
; Sets a 1 to 1 mapping up to 32
mmu_setup:
#define PAGE_TABLE_SIZE 32
    MOV r0 0    ; Page table index
mmu_start:
    CMP r0 PAGE_TABLE_SIZE
    < MOV @r0 r0
    INC r0
    < JMP mmu_start
    RETURN


; Peripheral setup
perf_setup:
#define perf_index  r0
#define perf_offset r1
#define perf_store  r2
    
    MOV perf_index  0    ; Perf index
    MOV perf_store  PAGE_TABLE_SIZE   ; Index to store the results from the IO_ASSIGNs
    MOV perf_offset 32   ; Perf memory offset
    ADD perf_offset 32
perf_start:
    ; Send general port status, returns:
    ;  1: Attached and assigned
    ;  0: Attached but not assigned
    ; -1: Nothing attached
    ; -2: Out of bounds for peripherals
    ; -3: Other error
#define perf_result r3
    IO_SEND perf_result perf_index 0
    CMP perf_result 0
    == JMP perf_assign  ; Unassigned
    PRINT "Skippd jmp" 
    INC perf_index
    > JMP perf_start    ; Assigned
    
    CMP perf_result -1           ; Check if attached
    == JMP perf_start   ; Not attached, skip
    < JMP perf_end      ; At the end of the perfs

perf_assign:
#define perf_memory_result r4
    IO_SEND perf_memory_result perf_index 1     ; Ask the peripheral how much memory it wants.
    
    MOV @perf_store perf_offset          ; Store the memory offset of the perf
    INC perf_store
    MOV @perf_store perf_memory_result   ; Store the amount of memory the perf wanted.
    INC perf_store
    
    IO_ASSIGN perf_index perf_offset perf_memory_result
    ADD perf_offset perf_memory_result
    INC perf_index
    JMP perf_start

perf_end:
    RETURN

; Main program
main:
    CALL mmu_setup
    CALL perf_setup

    MOV r5 32
    MOV r0 @r5          ; Grab the offset of the first perf
    INC r5
    MOV r1 @r5          ; Grab the size of the first perf

; Write test
    MOV r2 0
    MOV r3 5
mem_start:
    CMP r2 r1
    >= JMP mem_end
    MOV @r0 r3
    ADD r3 5
    INC r2
    INC r0
    JMP mem_start

mem_end:
    IO_SEND 0 0x12

    MOV r0 0
start:
    CMP r0 0x00FFFFFF
    ;CMP r1 10
    < INC r0
    < JMP start

end:
    PUSH 5
    PUSH 8
    CALL func_add
    POP r10
    PRINT "RESULT"
    PRINT r10
    PRINT "Finishing"
    HALT

