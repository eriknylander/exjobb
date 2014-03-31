#
# Makefile for CPP-2005
#

# Compiler and linker
CXX     = g++
CC      = g++

# Preprocessor, compiler and linker options
#CPPFLAGS  = -DNDEBUG
#CXXFLAGS  = -pipe -O2 -Wall -W -ansi -pedantic-errors
#CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast
CXXFLAGS = -I/home/erik/libemu/libemu/include -g
#CXXFLAGS += -ggdb
#LDFLAGS = -ggdb

# Sources and object files
SRC	= $(wildcard *.cpp)
OBJ	= $(SRC:.cpp=.o)

# Executables 
PROGS	= program

all: $(PROGS)

program: program.o parser.o instruction.o emulator.o /home/erik/libemu/libemu/src/.libs/libemu.a
	$(CXX) $(CXXFLAGS) program.o parser.o instruction.o emulator.o /home/erik/libemu/libemu/src/.libs/libemu.a -ludis86 -o program.out

parser: parser.o instruction.o
	$(CXX) $(CXXFLAGS) parser.o instruction.o -ludis86

clean:
	rm -f *.o

cleaner: clean
	rm -f $(PROGS) *.out

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

