#include <stdio.h>

#include "include_headers.h"
#include "test_jmp_call.h"

static void before()
{
  reset_cpu(cpu, memory);
}

static char* test_tax()
{
  start_test_info();
  //Given
  s32 cycles = 2;
  Byte value = 0x13;

  //Setup
  memory->memory_array[0xFFFC] = INS_TAX_IMP;
  cpu->Acc = value;
  cpu->X = 0;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("TAX should take 2 cycles", cycles == 0);
  mu_assert("TAX loaded accumulator into X", cpu->X == value);

  return 0;
}

static char* test_tay()
{
  start_test_info();
  //Given
  s32 cycles = 2;
  Byte value = 0xAC;

  //Setup
  memory->memory_array[0xFFFC] = INS_TAY_IMP;
  cpu->Acc = value;
  cpu->Y = 0;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("INS_TAY should take 2 cycles", cycles == 0);
  mu_assert("INS_TAY loaded accumulator into Y", cpu->Y == value);

  return 0;
}

static char* test_txa()
{
  start_test_info();
  //Given
  s32 cycles = 2;
  Byte value = 0xB1;

  //Setup
  memory->memory_array[0xFFFC] = INS_TXA_IMP;
  cpu->Acc = 0;
  cpu->X = value;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("INS_TXA should take 2 cycles", cycles == 0);
  mu_assert("INS_TXA loaded X into accumulator", cpu->Acc = value);

  return 0;
}

static char* test_tya()
{
  start_test_info();
  //Given
  s32 cycles = 2;
  Byte value = 0x94;

  //Setup
  memory->memory_array[0xFFFC] = INS_TYA_IMP;
  cpu->Acc = 0;
  cpu->Y = value;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("TYA should take 2 cycles", cycles == 0);
  mu_assert("TYA loaded Y into accumulator", cpu->X = value);

  return 0;
}

static char* all_transfers_test()
{
  before();
  mu_run_test(test_tax);

  before();
  mu_run_test(test_tay);

  before();
  mu_run_test(test_txa);

  before();
  mu_run_test(test_tya);

  return 0;
}


int run_all_transfers_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in)
{
  cpu = cpu_in;
  memory = memory_in;
  char* msg = all_transfers_test();
  if(msg != 0)
  {
    printf("%s\n", msg);
    fprintf(stderr,"Not all transfers tests passed\n--------\n");
    return -1;
  }

  printf("All transfers tests passed\n---------\n");
  return 0;
}
