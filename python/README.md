Documentation Generation
========================

Reference documentation for all the commands is generated via a python script.

It should be as simple as running

    python3 parseOpcodes.py

To get a new ../docs/reference.html file.

The actual code that is parsed is the 'Commands to types' section as that describes the
name of each command along with each form (argument signatures) of the commands.

Output
======

The order of the commands in the opCodes.cpp file is preserved.

Autolinks
=========

Anytime a sequence of uppercase letters are found that are also a known command (and it's not in a h1, h2, h3, h4 tag) then it's turned into a link to the docs for that command.

Metatags
========

Currently there are only two metatags for the documentation of a command

@group: Which gives a grouping to the command which hopefully makes it clearer the overall purpose of the command as well as groups the commands together in the list of commands.
Along with the general order of the listings of commands is preserved, so is the order in
which groups appear is preserved in the list of commands.

@short: A short description of the command for the list of commands.
