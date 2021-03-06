#include <time.h>

#include "data_types.h"
#include "memory.h"

#ifndef CPU_H
#define CPU_H

// The main CPU struct.
struct cpu_struct
{
  Word PC; //Program counter
  Word SP; //Stack pointer
  Byte Acc; //Accumulator
  Byte X, Y; //X and Y registers

  struct status_flags
  {
    // Bit 0, Carry flag
    Byte C : 1; 
    // Bit 1, Zero flag
    Byte Z : 1; 
    // Bit 2, Interrupt disable
    Byte I : 1; 
    // Bit 3, Decimal mode flag
    Byte D : 1; 
    // Bit 4, Break command
    Byte B : 1; 
    // Bit 5, Unused  
    Byte U : 1; 
    // Bit 6, Overflow flag
    Byte V : 1; 
    // Bit 7, Negative flag
    Byte N : 1; 
  } status_flags; // Bit field makes this struct only 8 bits and conforms to the original 6502

  // Pointer to the memory for the CPU - like a memory bus
  struct memory_struct* memory_bus;

  //--CPU clock parts--//
  u32 clock_time; //CPU clock time in ns (= 1/f)
  struct timespec last_clock_time;
  struct timespec new_time;
};

// Reset ops
void reset_cpu_word(struct cpu_struct *cpu, struct memory_struct *memory, Word reset_word);
void reset_cpu(struct cpu_struct *cpu, struct memory_struct *memory);

// Fetch operations
Byte fetch_byte(struct cpu_struct *cpu, s32* cycles);
Word fetch_word(struct cpu_struct *cpu, s32* cycles);

// Read operations
Byte read_byte(struct cpu_struct *cpu, s32* cycles, Word address);
Word read_word(struct cpu_struct *cpu, s32* cycles, Word address);

// Write operations
void write_byte(struct cpu_struct *cpu, s32* cycles, Word address, Byte value);
void write_word(struct cpu_struct *cpu, s32* cycles, Word address, Word value);

// Start execution
void execute(struct cpu_struct *cpu, s32* cycles);

#endif
