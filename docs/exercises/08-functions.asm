    ; A simple squaring function that also calls another function to 
    ; display the result of the squaring.
square:
    
    POP r10         ; Get the one and only argument.
    MUL r10 r10     ; Square the input

    PUSH r10        ; Push the result onto the stack so that the
                    ; output function can display it.
    CALL output

    PUSH r10        ; Push the result onto the stack so that the
                    ; code that called this function can get the reuslt.
    RETURN

output:
    POP r11         ; Get the only and only argument.

    PRINT "Output "
    PRINT r11       ; Display "Output = " + r11 + "\n"
    PRINT "\n"

    RETURN

main:
    PUSH 5
    CALL square     ; Call square function with the number 5.
    POP r0          ; Store 5 squared into register 0.

    PUSH 24
    CALL square     ; Call squaure function with the number 24.
    POP r1          ; Store 24 squared into register 1.

    ADD r2 r0 r1    ; Add the two squared results just for something to do.
    INFO r2         ; Output register 2 using the debug info command.
