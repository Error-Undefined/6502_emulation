
#include <stdio.h>

#include "test_clear.h"

static void before()
{
  reset_cpu(cpu, memory);
}

static char* test_clear_carry()
{
  start_test_info();

  //Given
  s32 cycles = 2;

  //Setup
  memory->memory_array[0xFFFC] = INS_CLC_IMP;
  cpu->status_flags.C = 1;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Clear carry should take 2 cycles", cycles == 0);
  mu_assert("Carry flag should be clear after INS_CLC", cpu->status_flags.C == 0);

  return 0;
}

static char* test_clear_decimal()
{
  start_test_info();

  //Given
  s32 cycles = 2;

  //Setup
  memory->memory_array[0xFFFC] = INS_CLD_IMP;
  cpu->status_flags.D = 1;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Clear decimal should take 2 cycles", cycles == 0);
  mu_assert("Carry flag should be clear after INS_CLD", cpu->status_flags.D == 0);

  return 0;
}

static char* test_clear_interrupt()
{
  start_test_info();

  //Given
  s32 cycles = 2;

  //Setup
  memory->memory_array[0xFFFC] = INS_CLI_IMP;
  cpu->status_flags.I = 1;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Clear interrupt should take 2 cycles", cycles == 0);
  mu_assert("Carry flag should be clear after INS_CLI", cpu->status_flags.I == 0);

  return 0;
}

static char* test_clear_overflow()
{
  start_test_info();

  //Given
  s32 cycles = 2;

  //Setup
  memory->memory_array[0xFFFC] = INS_CLV_IMP;
  cpu->status_flags.V = 1;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Clear overflow should take 2 cycles", cycles == 0);
  mu_assert("Carry flag should be clear after INS_CLV", cpu->status_flags.V == 0);

  return 0;
}

static char* test_set_carry()
{
  start_test_info();

  //Given
  s32 cycles = 2;

  //Setup
  memory->memory_array[0xFFFC] = INS_SEC_IMP;
  cpu->status_flags.C = 0;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Set carry should take 2 cycles", cycles == 0);
  mu_assert("Carry flag should be set after INS_SEC", cpu->status_flags.C == 1);

  return 0;
}

static char* test_set_decimal()
{
  start_test_info();

  //Given
  s32 cycles = 2;

  //Setup
  memory->memory_array[0xFFFC] = INS_SED_IMP;
  cpu->status_flags.D = 0;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Set decimal should take 2 cycles", cycles == 0);
  mu_assert("Decimal flag should be set after INS_SED", cpu->status_flags.D == 1);

  return 0;
}

static char* test_set_interrupt()
{
  start_test_info();

  //Given
  s32 cycles = 2;

  //Setup
  memory->memory_array[0xFFFC] = INS_SEI_IMP;
  cpu->status_flags.I = 0;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Set decimal should take 2 cycles", cycles == 0);
  mu_assert("Decimal flag should be set after INS_SEI", cpu->status_flags.I == 1);

  return 0;
}

static char* all_clear_test()
{
  before();
  mu_run_test(test_clear_carry);

  before();
  mu_run_test(test_clear_decimal);

  before();
  mu_run_test(test_clear_interrupt);

  before();
  mu_run_test(test_clear_overflow);

  before();
  mu_run_test(test_set_carry);

  before();
  mu_run_test(test_set_decimal);

  before();
  mu_run_test(test_set_interrupt);

  return 0;
}

int run_all_clear_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in)
{
  cpu = cpu_in;
  memory = memory_in;
  char* msg = all_clear_test();
  if(msg != 0)
  {
    printf("%s\n", msg);
    fprintf(stderr,"Not all clear tests passed\n--------\n");
    return -1;
  }

  printf("All clear tests passed\n---------\n");
  return 0;
}
