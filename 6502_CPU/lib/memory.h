#include "data_types.h"

#ifndef MEM_H
#define MEM_H

struct memory_struct
{
  // The 6502 accesses 64 kB of memory on an 16 bit bus (in hex 0xFFFF)
  Byte memory_array[MEM_LENTGH]; 
};

void initialize_memory(struct memory_struct* memory);

#endif
