
#include "data_types.h"
#include "memory.h"

void initialize_memory(struct memory_struct* memory)
{
  // Reset all memory bytes to 0
  for(int b = 0; b < MEM_LENTGH; ++b)
  {
    memory->memory_array[b] = 0;
  }
}

Byte read_byte(struct memory_struct* memory, LongWord address)
{
  return memory->memory_array[address];
}

Byte write_byte(struct memory_struct* memory, LongWord address, Byte b)
{
  Byte old_value = memory->memory_array[address];
  memory->memory_array[address] = b;
  return old_value;
}
