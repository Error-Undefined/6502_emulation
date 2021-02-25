
#include <stdio.h>

#include "include_headers.h"
#include "test_stack.h"

static void before()
{
  reset_cpu(cpu, memory);
}

static char* test_tsx()
{
  start_test_info();
  //Given
  s32 cycles =  2;

  //Setup
  memory->memory_array[0xFFFC] = INS_TSX_IMP;
  cpu->X = 0;
  cpu->SP = 0x42;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("TSX should take 2 cycles", cycles == 0);
  mu_assert("X register should be loaded with stack pointer", cpu->X == 0x42);

  return 0;
}

static char* test_txs()
{
  start_test_info();
  //Given
  s32 cycles =  2;

  //Setup
  memory->memory_array[0xFFFC] = INS_TXS_IMP;
  cpu->X = 0x23;
  cpu->SP = 0x42;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("TXS should take 2 cycles", cycles == 0);
  mu_assert("Stack pointer should be loaded with X register", cpu->SP == 0x23);

  return 0;
}

static char* test_pha()
{
  start_test_info();

  //Given
  s32 cycles = 3;
  Byte value = 0x54;

  //Setup
  memory->memory_array[0xFFFC] = INS_PHA_IMP;
  memory->memory_array[0x1FF] = 0; // Top of stack
  cpu->Acc = value;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_PHA should take 3 cycles", cycles == 0);
  mu_assert("INS_PHA should load the accumulator on the stack", memory->memory_array[0x1FF] == value);
  mu_assert("INS_PHA should decrement the stack pointer", cpu->SP = 0xFE);

  return 0;
}

static char* test_pla()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value = 0x80;

  //Setup
  memory->memory_array[0xFFFC] = INS_PHA_IMP;
  memory->memory_array[0x1FF] = value; // Top of stack
  cpu->Acc = 0;
  cpu->SP = 0xFE; //Already decremented by 1 for test

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_PLA should take 4 cycles", cycles == 0);
  mu_assert("INS_PLA should pull the accumulator from the stack", cpu->Acc == value);
  mu_assert("INS_PLA should increment the stack pointer", cpu->SP = 0xFF);

  return 0;
}

static char* test_php()
{
  start_test_info();

  //Given
  s32 cycles = 3;

  //Setup
  cpu->status_flags.C = 1;
  cpu->status_flags.Z = 0;
  cpu->status_flags.I = 0;
  cpu->status_flags.D = 0;
  cpu->status_flags.B = 0;
  cpu->status_flags.U = 1;
  cpu->status_flags.V = 1;
  cpu->status_flags.N = 1;
  //In hex these become 0xe1

  memory->memory_array[0xFFFC] = INS_PHP_IMP;
  cpu->SP = 0xFF;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_PHP should take 3 cycles", cycles == 0);
  mu_assert("INS_PHP should properly push the processor status on the stack", memory->memory_array[0x1FF] == 0xE1);
  mu_assert("Stack pointer should be decremented by INS_PHP", cpu->SP = 0xFE);

  return 0;
}

static char* test_plp()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  //In binary: 0b10100110. Byte i can be extracted by doing (0xA6 >> i & 1)
  Byte value_on_stack = 0xA6; 

  //Setup
  cpu->status_flags.C = 0;
  cpu->status_flags.Z = 0;
  cpu->status_flags.I = 0;
  cpu->status_flags.D = 0;
  cpu->status_flags.B = 0;
  cpu->status_flags.U = 1;
  cpu->status_flags.V = 0;
  cpu->status_flags.N = 0;
  

  memory->memory_array[0xFFFC] = INS_PLP_IMP;
  memory->memory_array[0x1FF] = value_on_stack;
  cpu->SP = 0xFE;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_PLP should take 4 cycles", cycles == 0);
  mu_assert("Stack pointer should be incremented by INS_PHP", cpu->SP = 0xFE);
  mu_assert("C flag properly set", (value_on_stack >> 0 & 1) == cpu->status_flags.C);
  mu_assert("Z flag properly set", (value_on_stack >> 1 & 1) == cpu->status_flags.Z);
  mu_assert("I flag properly set", (value_on_stack >> 2 & 1) == cpu->status_flags.I);
  mu_assert("D flag properly set", (value_on_stack >> 3 & 1) == cpu->status_flags.D);
  mu_assert("B flag properly set", (value_on_stack >> 4 & 1) == cpu->status_flags.B);
  mu_assert("U flag properly set", (value_on_stack >> 5 & 1) == cpu->status_flags.U);
  mu_assert("V flag properly set", (value_on_stack >> 6 & 1) == cpu->status_flags.V);
  mu_assert("N flag properly set", (value_on_stack >> 7 & 1) == cpu->status_flags.N);

  return 0;
}

static char* all_stack_test()
{
  before();
  mu_run_test(test_tsx);

  before();
  mu_run_test(test_txs);

  before();
  mu_run_test(test_pha);

  before();
  mu_run_test(test_php);

  before();
  mu_run_test(test_pla);

  before();
  mu_run_test(test_plp);

  return 0;
}

int run_all_stack_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in)
{
  cpu = cpu_in;
  memory = memory_in;
  char* msg = all_stack_test();
  if(msg != 0)
  {
    printf("%s\n", msg);
    fprintf(stderr,"Not all stack tests passed\n--------\n");
    return -1;
  }

  printf("All stack tests passed\n---------\n");
  return 0;
}
