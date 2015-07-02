main:    ; The stack starts empty.
    PUSH 1      ; The stack now contains [ 1 ]
    PUSH 2      ; The stack now contains [ 1, 2 ]
    POP r1      ; The stack now contains [ 1 ]
    POP r2      ; The stack is now empty.
    INFO r1     ; Prints 2
    INFO r2     ; Prints 1
