# Copyright J.Carruthers
# Copyright 2021 Aily Walker aily@bu.edu
# This Makefile
# gives instructions to the command
# make
# about how and whether to build programs
#
# The syntax is

# thingtobuld:  what-it-needs-or-depends-on
#    instructions on how to build it
#

# make uses the modification dates to determine
# is the programs need to be re-built

all: slot

slot: slot_gui.cpp
	g++ slot_gui.cpp -o slot -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
