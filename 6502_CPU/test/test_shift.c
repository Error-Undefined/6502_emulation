
#include <stdio.h>

#include "test_shift.h"

static void before()
{
  reset_cpu(cpu, memory);
}

/* ASL tests */
static char* test_asl_acc()
{
  start_test_info();

  s32 cycles = 2;
  Byte acc_value = 0x4A;
  Byte acc_value_expected  = 2 * 0x4A;

  //Setup 
  memory->memory_array[0xFFFC] = INS_ASL_ACC;
  cpu->Acc = acc_value;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Accumulator not shifted left by 1", cpu->Acc == acc_value_expected);
  mu_assert("INS_ASL_ACC should takes two cycles", cycles == 0);

  return 0;
}

static char* test_asl_zp()
{
  start_test_info();

  s32 cycles = 5;
  Byte acc_value = 0x4A;
  Byte acc_value_expected  = 2 * 0x4A;

  Byte addr_zp = 0x45;

  //Setup
  memory->memory_array[0xFFFC] = INS_ASL_ZP;
  memory->memory_array[0xFFFD] = addr_zp;
  memory->memory_array[addr_zp] = acc_value;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_ASL_ZP should take 5 cycles", cycles == 0);
  mu_assert("Value in zeropage shifted left", memory->memory_array[addr_zp] == acc_value_expected);

  return 0;
}

static char* all_shift_test()
{
  before();
  mu_run_test(test_asl_acc);

  before();
  mu_run_test(test_asl_zp);

  return 0;
}

int run_all_shift_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in)
{
  cpu = cpu_in;
  memory = memory_in;
  char* msg = all_shift_test();
  if(msg != 0)
  {
    printf("%s\n", msg);
    fprintf(stderr,"Not all shift tests passed\n--------\n");
    return -1;
  }

  printf("All shift tests passed\n---------\n");
  return 0;
}

