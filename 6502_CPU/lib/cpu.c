#include <stdio.h>

#include "data_types.h"

#include "cpu.h"
#include "memory.h"
#include "instructions.h"


void reset_cpu(struct cpu_struct* cpu, struct memory_struct* memory)
{
  reset_cpu_word(cpu, memory, 0xFFFC);
}

void reset_cpu_word(struct cpu_struct* cpu, struct memory_struct* memory, Word reset_word)
{ 
  // Program counter and stack pointer
  cpu->PC = reset_word;
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

  cpu->memory_bus = memory;

  initialize_memory(memory);
}


// Fetch operations
Byte fetch_byte(struct cpu_struct *cpu, s32* cycles)
{
  Byte to_fetch = cpu->memory_bus->memory_array[cpu->PC];
  ++cpu->PC;
  *(cycles) = *(cycles) - 1;
  return to_fetch;
}

Word fetch_word(struct cpu_struct *cpu, s32* cycles)
{
  Byte lower_byte = fetch_byte(cpu, cycles);
  Byte higher_byte = fetch_byte(cpu, cycles);

  Word to_fetch = lower_byte | (higher_byte << 8);

  return to_fetch;
}

// Read operations
Byte read_byte(struct cpu_struct *cpu, s32* cycles, Word address)
{
  Byte to_read = cpu->memory_bus->memory_array[address];
  *(cycles) = *(cycles) - 1;
  return to_read;
}

Word read_word(struct cpu_struct *cpu, s32* cycles, Word address)
{
  Byte lower_byte = read_byte(cpu, cycles, address);
  Byte higher_byte = read_byte(cpu, cycles, address + 1);

  Word to_read = lower_byte | (higher_byte << 8);
  return to_read;
}

// Write operations
void write_byte(struct cpu_struct *cpu, s32* cycles, Word address, Byte value)
{
  cpu->memory_bus->memory_array[address] = value;
  *(cycles) = *(cycles) - 1;
}

void write_word(struct cpu_struct *cpu, s32* cycles, Word address, Word value)
{
  Byte lower_value = value & 0xFF;
  Byte higher_value = value >> 8;
  write_byte(cpu, cycles, address, lower_value);
  write_byte(cpu, cycles, address, higher_value);
}

void load_to_register(struct cpu_struct* cpu, Byte* register_to_load, Byte value)
{
  *register_to_load = value;
  cpu->status_flags.Z = ~value;
  cpu->status_flags.N = (value >> 6);
}


void execute(struct cpu_struct *cpu, s32* cycles)
{ 
  //While the number of cycles is still > 0
  while(*cycles > 0)
  {
    // Fetch the next instruction from the PC
    Byte instruction = fetch_byte(cpu, cycles);
    switch (instruction)
    {
      
      case INS_LDA_IM:
      {
        Byte load = fetch_byte(cpu, cycles);
        load_to_register(cpu, &cpu->Acc, load);
        break;
      }
      case INS_LDA_ZP:
      {
        Byte addr_in_zp = fetch_byte(cpu, cycles);
        Byte load = read_byte(cpu, cycles, addr_in_zp);
        load_to_register(cpu, &cpu->Acc, load);
        break;
      }
      case INS_LDA_ZP_X:
      {
        Byte addr_in_zp = fetch_byte(cpu, cycles);
        
        addr_in_zp += cpu->X;
        *(cycles) = *(cycles) - 1; // Consume an extra cycle

        Byte load = read_byte(cpu, cycles, addr_in_zp);
        load_to_register(cpu, &cpu->Acc, load);
        break;
      }
      case INS_LDA_ABS:
      {
        Word addr_absolute = fetch_word(cpu, cycles);
        Byte load = read_byte(cpu, cycles, addr_absolute);
        load_to_register(cpu, &cpu->Acc, load);
        break;
      }


      default:
        printf("Found instruction %x, not implemented! Returning from execution\n", instruction);
        return;
        break;
    }

  }
}
