
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
