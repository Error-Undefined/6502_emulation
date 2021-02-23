# Define the compiler and the linker. The linker must be defined since
# the implicit rule for linking uses CC as the linker. g++ can be
# changed to clang++.
CXX = gcc
CC  = $(CXX)

# Generate dependencies in *.d files
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of GNU's libstdc++.
# -g is for debugging.
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast 
CXXFLAGS += -g
CXXFLAGS += $(DEPFLAGS)
LDFLAGS = -g 
#CPPFLAGS += -stdlib=libc++
#CXXFLAGS += -stdlib=libc++
#LDFLAGS +=  -stdlib=libc++


# Targets
PROGS = all_tests
all: $(PROGS)

# Targets rely on implicit rules for compiling and linking
all_tests: all_tests.o ../lib/cpu.o ../lib/memory.o test_lda.o

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
