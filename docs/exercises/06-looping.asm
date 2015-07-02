main:
    MOV r0 1                ; for (var i = 1     loop setup
start_of_loop:
    CMP r0 10               ; i <= 10             loop conditional
    > JMP end_of_loop
                            ; { 
    PRINT r0                ;
    PRINT "\n"              ;    console.log(i);
                            ; }
 
    INC r0                  ; i++)               loop incrementer
    JMP start_of_loop 

end_of_loop:
    PRINT "Loop complete\n" ; console.log("Loop complete")
