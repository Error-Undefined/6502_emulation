
#include "data_types.h"

#include "cpu.h"
#include "memory.h"


void initialize_cpu(struct cpu_struct* cpu, struct memory_struct* memory)
{ 
  // Program counter and stack pointer
  cpu->PC = 0xFFFC;
  cpu->SP = 0xFF;

  // Accumulator, X and Y
  cpu->Acc = 0;
  cpu->X = 0;
  cpu->Y = 0;

  //Status flags, set all but unused to 0
  cpu->status_flags.C = 0;
  cpu->status_flags.Z = 0;
  cpu->status_flags.I = 0;
  cpu->status_flags.D = 0;
  cpu->status_flags.B = 0;
  cpu->status_flags.V = 0;
  cpu->status_flags.N = 0;

  initialize_memory(memory);
}