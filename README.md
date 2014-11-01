moss
====

Funky MOSS to the rescue.

MOSS is my attempt at creating a VM that follows mostly how a real computer works whilst keeping things simple.

The end goal however is to know enough how it all works to make a more simplifed VM that works well for creating a high level langage and OS for controlling the computer.

Currently being influenced mostly by x86 but also by ARM to some degree.

Building
========

Should build fine with any g++ version that supports c++0x

    make
    ./Moss

Defaults to building in debug mode, to build for release:

    make RELEASE=yes
    ./Moss

In theory should work with MSVC but hasn't been tested.
