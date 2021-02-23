# Define the compiler and the linker.
CXX = gcc
CC  = $(CXX)

# Generate dependencies in *.d files
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

# Define preprocessor, compiler, and linker flags.
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast 
CXXFLAGS += -g
CXXFLAGS += $(DEPFLAGS)
LDFLAGS = -g 


# Targets
PROGS = cpu.o memory.o
all: $(PROGS)

# Targets rely on implicit rules for compiling and linking

# Phony targets
.PHONY: all test clean

clean_objects:
	rm -f *.o

# Standard clean
clean:
	rm -f *.o $(PROGS)

# Also clean .d files
depclean: clean
	rm -f *.d


# Include the *.d files
SRC = $(wildcard *.c)
-include $(SRC:.c=.d)
