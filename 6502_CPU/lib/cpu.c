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
  cpu->status_flags.U = 1;
  cpu->status_flags.V = 0;
  cpu->status_flags.N = 0;

  cpu->memory_bus = memory;

  initialize_memory(memory);
}

// Consumes a cycle from the clock
static void consume_cycle(s32* cycles)
{
  *(cycles) = *(cycles) - 1;
}

static void consume_cycles(s32* cycles, int nbr)
{
  *(cycles) = *(cycles) - nbr;
}

// Fetch operations
Byte fetch_byte(struct cpu_struct *cpu, s32* cycles)
{
  Byte to_fetch = cpu->memory_bus->memory_array[cpu->PC];
  ++cpu->PC;
  consume_cycle(cycles);
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
  consume_cycle(cycles);
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
  consume_cycle(cycles);
}

void write_word(struct cpu_struct *cpu, s32* cycles, Word address, Word value)
{
  Byte lower_value = value & 0xFF;
  Byte higher_value = value >> 8;
  write_byte(cpu, cycles, address, lower_value);
  write_byte(cpu, cycles, address + 1, higher_value);
}

//Loads a byte to the specified register. Sets the zero flag and negative flag if applicable
void load_to_register(struct cpu_struct* cpu, Byte* register_to_load, Byte value)
{
  *register_to_load = value;
  cpu->status_flags.Z = ~value;
  cpu->status_flags.N = (value >> 6);
}

/* Addressing modes */

//Consumes 1 clock cycle
static Byte address_zero_page(struct cpu_struct* cpu, s32* cycles)
{
  Byte address_in_zp = fetch_byte(cpu, cycles);
  return address_in_zp;
}

// Consumes 2 clock cycles
static Byte address_zero_page_x(struct cpu_struct* cpu, s32* cycles)
{
  Byte address_in_zp = fetch_byte(cpu, cycles);
  address_in_zp = (address_in_zp + cpu->X) & 0xFF;
  consume_cycle(cycles);
  return address_in_zp;
}

// Consumes 2 clock cycles
static Byte address_zero_page_y(struct cpu_struct* cpu, s32* cycles)
{
  Byte address_in_zp = fetch_byte(cpu, cycles);
  address_in_zp = (address_in_zp + cpu->Y) & 0xFF;
  consume_cycle(cycles);
  return address_in_zp;
}

// Consumes 2 clock cycles
static Word address_absolute(struct cpu_struct* cpu, s32* cycles)
{
  Word address_absolute = fetch_word(cpu, cycles);
  return address_absolute;
}

// Consumes 2 clock cycles, or 3 if check_page_bounds is set and a page bound is crossed
static Word address_absolute_x(struct cpu_struct* cpu, s32* cycles, int check_page_bounds)
{
  Word addr_absolute = fetch_word(cpu, cycles);
  // Test if we cross a page boundary (Higher byte after addition is different) and consume a cycle
  if(check_page_bounds && ((addr_absolute >> 8) ^ ((addr_absolute + cpu->X) >> 8)))
  {
    consume_cycle(cycles);
  }
  addr_absolute += cpu->X;
  return addr_absolute;
}

// Consumes 2 clock cycles, or 3 if check_page_bounds is set and a page bound is crossed
static Word address_absolute_y(struct cpu_struct* cpu, s32* cycles, int check_page_bounds)
{
  Word addr_absolute = fetch_word(cpu, cycles);
  // Test if we cross a page boundary (Higher byte after addition is different) and consume a cycle
  if(check_page_bounds && ((addr_absolute >> 8) ^ ((addr_absolute + cpu->Y) >> 8)))
  {
    consume_cycle(cycles);
  }
  addr_absolute += cpu->Y;
  return addr_absolute;
}

// Consumes 4 clock cycles
static Word address_indirect(struct cpu_struct* cpu, s32* cycles)
{
  Word addr_indirect = fetch_word(cpu, cycles);
  Word value_address = read_word(cpu, cycles, addr_indirect);
  return value_address;
}

// Indexed indirect always with X register. 
// Consumes 4 clock cycles.
static Word address_indexed_indirect(struct cpu_struct* cpu, s32* cycles)
{
  Byte address_ind = fetch_byte(cpu, cycles);
  //Wrap to zero page and consume a cycle
  address_ind = (address_ind + cpu->X) & 0xFF;
  consume_cycle(cycles);
  Word value_address = read_word(cpu, cycles, address_ind);
  return value_address;
}

// Indirect indexed always with Y register. 
// Consumes 3 clock cycles, or 4 if check_page_bounds is set and a page bound is crossed 
static Word address_indirect_indexed(struct cpu_struct* cpu, s32* cycles, int check_page_boundary)
{
  Byte address_ind = fetch_byte(cpu, cycles); 
  Word address_from_zp = read_word(cpu, cycles, address_ind);
  //Add Y register. Test if we cross a page boundary and then consume an extra cycle
  if(check_page_boundary && ((address_from_zp >> 8) ^ ((address_from_zp + cpu->Y) >> 8)))
  {
    consume_cycle(cycles);
  }
  address_from_zp = address_from_zp + cpu->Y;
  return address_from_zp;
}

/* Stack operations */
// Pushes a byte on the stack. The stack pointer is decremented.
// Consumes 1 clock cycle
static void push_byte_to_stack(struct cpu_struct* cpu, s32* cycles, Byte value)
{
  Word stack_addr = 0x100 | cpu->SP;
  write_byte(cpu, cycles, stack_addr, value);
  cpu->SP = cpu->SP - 1;

}

// Pushes a word on the stack. The higher byte is pushed first, followed by the lower byte. 
// The stack pointer is decremented twice.
// Consumes 2 clock cycles
static void push_word_to_stack(struct cpu_struct* cpu, s32* cycles, Word value)
{
  Byte higher_byte = value >> 8;
  Byte lower_byte = value & 0xFF;
  push_byte_to_stack(cpu, cycles, higher_byte);
  push_byte_to_stack(cpu, cycles, lower_byte);
}

// Pops a byte from the stack. The stack pointer is incremented.
// Consumes 1 clock cycle
static Byte pop_byte_from_stack(struct cpu_struct* cpu, s32* cycles)
{
  cpu->SP = cpu->SP + 1;
  Word stack_addr = 0x100 | cpu->SP;
  Byte popped_byte = read_byte(cpu, cycles, stack_addr);
  return popped_byte;
}

// Pops a word from the stack. The stack pointer is incremented.
// Consumes 2 clock cycles
static Word pop_word_from_stack(struct cpu_struct* cpu, s32* cycles)
{
  Byte lower_byte = pop_byte_from_stack(cpu, cycles);
  Byte higher_byte = pop_byte_from_stack(cpu, cycles);
  Word popped_word = (higher_byte << 8) & 0xFF00;
  popped_word = popped_word + lower_byte;
  return popped_word;
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
      //--LDA--//
      case INS_LDA_IM:
      {
        Byte load = fetch_byte(cpu, cycles);
        load_to_register(cpu, &cpu->Acc, load);
        break;
      }
      case INS_LDA_ZP:
      {
        Byte addr_in_zp = address_zero_page(cpu, cycles);
        Byte load = read_byte(cpu, cycles, addr_in_zp);
        load_to_register(cpu, &cpu->Acc, load);
        break;
      }
      case INS_LDA_ZP_X:
      {
        Byte addr_in_zp = address_zero_page_x(cpu, cycles);
        Byte load = read_byte(cpu, cycles, addr_in_zp);
        load_to_register(cpu, &cpu->Acc, load);
        break;
      }
      case INS_LDA_ABS:
      {
        Word addr_absolute = address_absolute(cpu, cycles);
        Byte load = read_byte(cpu, cycles, addr_absolute);
        load_to_register(cpu, &cpu->Acc, load);
        break;
      }
      case INS_LDA_ABS_X:
      {
        Word addr_absolute = address_absolute_x(cpu, cycles, 1);   
        Byte load = read_byte(cpu, cycles, addr_absolute);
        load_to_register(cpu, &cpu->Acc, load);
        break;
      }
      case INS_LDA_ABS_Y:
      {
        Word addr_absolute = address_absolute_y(cpu, cycles, 1);      
        Byte load = read_byte(cpu, cycles, addr_absolute);
        load_to_register(cpu, &cpu->Acc, load);
        break;
      }
      case INS_LDA_IND_X:
      {        
        Word address = address_indexed_indirect(cpu, cycles);
        Byte value = read_byte(cpu, cycles, address);
        load_to_register(cpu, &cpu->Acc, value);
        break;
      }
      case INS_LDA_IND_Y:
      {
        Word address_from_zp = address_indirect_indexed(cpu, cycles, 1);
        Byte value = read_byte(cpu, cycles, address_from_zp);
        load_to_register(cpu, &cpu->Acc, value);
        break;
      }
      //--LDX--//
      case INS_LDX_IM:
      {
        Byte load = fetch_byte(cpu, cycles);
        load_to_register(cpu, &cpu->X, load);
        break;
      }
      case INS_LDX_ZP:
      {
        Byte addr_in_zp = address_zero_page(cpu, cycles);
        Byte load = read_byte(cpu, cycles, addr_in_zp);
        load_to_register(cpu, &cpu->X, load);
        break;
      }
      case INS_LDX_ZP_Y:
      {
        Byte addr_in_zp = address_zero_page_y(cpu, cycles);
        Byte load = read_byte(cpu, cycles, addr_in_zp);
        load_to_register(cpu, &cpu->X, load);
        break;
      }
      case INS_LDX_ABS:
      {
        Word addr_absolute = address_absolute(cpu, cycles);
        Byte load = read_byte(cpu, cycles, addr_absolute);
        load_to_register(cpu, &cpu->X, load);
        break;
      }
      case INS_LDX_ABS_Y:
      {
        Word addr_absolute = address_absolute_y(cpu, cycles, 1);      
        Byte load = read_byte(cpu, cycles, addr_absolute);
        load_to_register(cpu, &cpu->X, load);
        break;
      }
      //--LDY--//
      case INS_LDY_IM:
      {
        Byte load = fetch_byte(cpu, cycles);
        load_to_register(cpu, &cpu->Y, load);
        break;
      }
      case INS_LDY_ZP:
      {
        Byte addr_in_zp = address_zero_page(cpu, cycles);
        Byte load = read_byte(cpu, cycles, addr_in_zp);
        load_to_register(cpu, &cpu->Y, load);
        break;
      }
      case INS_LDY_ZP_X:
      {
        Byte addr_in_zp = address_zero_page_x(cpu, cycles);
        Byte load = read_byte(cpu, cycles, addr_in_zp);
        load_to_register(cpu, &cpu->Y, load);
        break;
      }
      case INS_LDY_ABS:
      {
        Word addr_absolute = address_absolute(cpu, cycles);
        Byte load = read_byte(cpu, cycles, addr_absolute);
        load_to_register(cpu, &cpu->Y, load);
        break;
      }
      case INS_LDY_ABS_X:
      {
        Word addr_absolute = address_absolute_x(cpu, cycles, 1);     
        Byte load = read_byte(cpu, cycles, addr_absolute);
        load_to_register(cpu, &cpu->Y, load);
        break;
      }
      //--STA--//
      case INS_STA_ZP:
      {
        Byte addr_in_zp = address_zero_page(cpu, cycles);
        write_byte(cpu, cycles, addr_in_zp, cpu->Acc);
        break;
      }
      case INS_STA_ZP_X:
      {
        Byte addr_in_zp = address_zero_page_x(cpu, cycles);
        write_byte(cpu, cycles, addr_in_zp, cpu->Acc);
        break;
      }
      case INS_STA_ABS:
      {
        Word addr_absolute = address_absolute(cpu, cycles);
        write_byte(cpu, cycles, addr_absolute, cpu->Acc);
        break;
      }
      case INS_STA_ABS_X:
      {
        Word addr_absolute = address_absolute_x(cpu, cycles, 0);
        write_byte(cpu, cycles, addr_absolute, cpu->Acc);
        consume_cycle(cycles);
        break;
      }
      case INS_STA_ABS_Y:
      {
        Word addr_absolute = address_absolute_y(cpu, cycles, 0);
        write_byte(cpu, cycles, addr_absolute, cpu->Acc);
        consume_cycle(cycles);
        break;
      }
      case INS_STA_IND_X:
      {
        Word value_address = address_indexed_indirect(cpu, cycles);
        write_byte(cpu, cycles, value_address, cpu->Acc);
        break;
      }
      case INS_STA_IND_Y:
      {
        Word addr_indirect_word = address_indirect_indexed(cpu, cycles, 0);
        write_byte(cpu, cycles, addr_indirect_word, cpu->Acc);
        consume_cycle(cycles);
        break;
      }
      //--STX--//
      case INS_STX_ZP:
      {
        Byte addr_zeropage = address_zero_page(cpu, cycles);
        write_byte(cpu, cycles, addr_zeropage, cpu->X);
        break;
      }
      case INS_STX_ZP_Y:
      {
        Byte addr_zeropage = address_zero_page_y(cpu, cycles);
        write_byte(cpu, cycles, addr_zeropage, cpu->X);
        break;
      }
      case INS_STX_ABS:
      {
        Word addr_absolute = address_absolute(cpu, cycles);
        write_byte(cpu, cycles, addr_absolute, cpu->X);
        break;
      }
      //--STY--//
      case INS_STY_ZP:
      {
        Byte addr_zeropage = address_zero_page(cpu, cycles);
        write_byte(cpu, cycles, addr_zeropage, cpu->Y);
        break;
      }
      case INS_STY_ZP_X:
      {
        Byte addr_zeropage = address_zero_page_x(cpu, cycles);
        write_byte(cpu, cycles, addr_zeropage, cpu->Y);
        break;
      }
      case INS_STY_ABS:
      {
        Word addr_absolute = address_absolute(cpu, cycles);
        write_byte(cpu, cycles, addr_absolute, cpu->Y);
        break;
      }
      //--TAX--//
      case INS_TAX_IMP:
      {
        load_to_register(cpu, &cpu->X, cpu->Acc);
        consume_cycle(cycles);
        break;
      }
      //--TAY--//
      case INS_TAY_IMP:
      {
        load_to_register(cpu, &cpu->Y, cpu->Acc);
        consume_cycle(cycles);
        break;
      }
      //--TXA--//
      case INS_TXA_IMP:
      {
        load_to_register(cpu, &cpu->Acc, cpu->X);
        consume_cycle(cycles);
        break;
      }
      //--TYA--//
      case INS_TYA_IMP:
      {
        load_to_register(cpu, &cpu->Acc, cpu->Y);
        consume_cycle(cycles);
        break;
      }
      //--JMP--//
      case INS_JMP_ABS:
      {
        Word addr_absolute = address_absolute(cpu, cycles);
        cpu->PC = addr_absolute;
        break;
      }
      case INS_JMP_IND:
      {
        Word addr_indirect = address_indirect(cpu, cycles);
        cpu->PC = addr_indirect;
        break;
      }
      //--JSR--//
      case INS_JSR_ABS:
      {
        Word addr_absolute = address_absolute(cpu, cycles);
        push_word_to_stack(cpu, cycles, cpu->PC - 1);
        cpu->PC = addr_absolute;
        consume_cycle(cycles);
        break;
      }
      //--RTS--//
      case INS_RTS_IMP:
      {
        cpu->PC = pop_word_from_stack(cpu, cycles);
        cpu->PC = cpu->PC + 1;
        consume_cycles(cycles, 3);
        break;
      }


      //--System instructions--//
      case INS_NOP:
      {
        consume_cycle(cycles);
        break;
      }

      default:
        printf("Found instruction 0x%x, not implemented! Returning from execution\n", instruction);
        *(cycles) = *(cycles) + 1;
        printf("Clock cycles left: %d\n", *(cycles));
        return;
        break;
    }

  }
}
