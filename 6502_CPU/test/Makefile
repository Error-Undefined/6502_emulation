# Define the compiler and the linker.
CXX = gcc
CC  = $(CXX)

# Generate dependencies in *.d files
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

# Define preprocessor, compiler, and linker flags.
CFLAGS =  -Og -Wall -Wextra -pedantic-errors -Wold-style-cast 
CFLAGS += -g
CFLAGS += $(DEPFLAGS)
LDFLAGS = -g 


# Targets
PROGS = all_tests
all: $(PROGS)

TESTS = test_lda.o test_ldx_ldy.o test_sta.o test_sys.o test_stx_sty.o test_jmp_call.o

# Targets rely on implicit rules for compiling and linking
all_tests: all_tests.o ../lib/cpu.o ../lib/memory.o $(TESTS)

# Phony targets
.PHONY: all test clean

# Standard clean
clean:
	rm -f *.o $(PROGS)
	rm ../lib/*.o

# Also clean .d files
depclean: clean
	rm -f *.d
	rm ../lib/*.d


# Include the *.d files
SRC = $(wildcard *.c)
-include $(SRC:.c=.d)
