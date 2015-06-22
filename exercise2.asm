main:
    PRINT "Input something will ya: "
    INPUT r0
    PRINT "\nYou inputted: "
    PRINT r0

    CMP r0 100
    >= PRINT "\nToo old\n"
    >= JMP end

    CMP r0 20
    >= PRINT "\nJust right\n"
    >= JMP end

    PRINT "\nToo young\n"

end:
    PRINT "\nDone!\n"

