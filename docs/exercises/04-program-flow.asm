main:
    PRINT "Hello there, how are you?\n"
    JMP one_sided_conversation                ; This will jump to the one_sided_conversation label, 
                                              ; thus skipping the next PRINT command, 
                                              ; giving us a one-sided conversation.
    PRINT "I'm good thanks, how are you?\n"

  one_sided_conversation:
    PRINT "I'm good thank you.\n"
