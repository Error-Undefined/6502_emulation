#include <stdio.h>

#include "include_headers.h"
#include "test_sys.h"

static void before()
{
  reset_cpu(cpu, memory);
}

static char* test_nop()
{
  start_test_info();
  //Given
  s32 cycles = 4;

  //Setup
  memory->memory_array[0xFFFC] = INS_NOP;
  memory->memory_array[0xFFFD] = INS_NOP;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("Two NOP should consume 4 cycles", cycles == 0);


  return 0;
}

static char* test_brk()
{
  start_test_info();
  //Given
  reset_cpu_word(cpu, memory, 0x8000);

  s32 cycles = 7;
  Byte lower_interrupt_address = 0x45;
  Byte higher_interrupt_address = 0xC1;
  Word interrupt_address = 0xC145;
  Byte processor_flags = 0xE2; //0b11100010

  //Setup
  memory->memory_array[0x8000] = INS_BRK_IMP;
  memory->memory_array[0xFFFE] = lower_interrupt_address;
  memory->memory_array[0xFFFF] = higher_interrupt_address;
  cpu->status_flags.C = 0;
  cpu->status_flags.Z = 1;
  cpu->status_flags.I = 0;
  cpu->status_flags.D = 0;
  cpu->status_flags.B = 0;
  cpu->status_flags.V = 1;
  cpu->status_flags.N = 1;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Break should take 7 cycles", cycles == 0);
  mu_assert("Break flag set after interrupt", cpu->status_flags.B == 1);
  mu_assert("Program counter should be interrupt address", cpu->PC == interrupt_address);
  mu_assert("Processor flags pushed onto the stack", memory->memory_array[0x1FF] = processor_flags);
  mu_assert("Old program counter pushed onto the stack", memory->memory_array[0x1FE] = 0x80);
  mu_assert("Old program counter pushed onto the stack", memory->memory_array[0x1FD] = 0x83);

  return 0;
}

static char* test_rti()
{
  start_test_info();
  //Given
  reset_cpu_word(cpu, memory, 0x8000);

  s32 cycles = 6;
  Byte lower_interrupt_address = 0x45;
  Byte higher_interrupt_address = 0xC1;
  Word interrupt_return_address = 0xC145;
  Byte processor_flags = 0xE2; //0b11100010

  //Setup
  memory->memory_array[0x8000] = INS_RTI_IMP;
  memory->memory_array[0x1FD] = processor_flags;
  memory->memory_array[0x1FE] = lower_interrupt_address;
  memory->memory_array[0x1FF] = higher_interrupt_address;
  cpu->SP = 0x1FC;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("RTI should take 6 cycles", cycles == 0);
  mu_assert("New program counter should be loaded from the stack", cpu->PC == interrupt_return_address);
  mu_assert("CPU flags should be set from stack", cpu->status_flags.C == 0);
  mu_assert("CPU flags should be set from stack", cpu->status_flags.Z == 1);
  mu_assert("CPU flags should be set from stack", cpu->status_flags.I == 0);
  mu_assert("CPU flags should be set from stack", cpu->status_flags.D == 0);
  mu_assert("CPU flags should be set from stack", cpu->status_flags.B == 0);
  mu_assert("CPU flags should be set from stack", cpu->status_flags.U == 1);
  mu_assert("CPU flags should be set from stack", cpu->status_flags.V == 1);
  mu_assert("CPU flags should be set from stack", cpu->status_flags.N == 1);

  return 0;
}

static char* all_sys_test()
{
  before();
  mu_run_test(test_nop);

  before();
  mu_run_test(test_brk);

  before();
  mu_run_test(test_rti);

  return 0;
}

int run_all_sys_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in)
{
  cpu = cpu_in;
  memory = memory_in;
  char* msg = all_sys_test();
  if(msg != 0)
  {
    printf("%s\n", msg);
    fprintf(stderr,"Not all system instructions tests passed\n--------\n");
    return -1;
  }

  printf("All system instruction tests passed\n---------\n");

  return 0;
}
