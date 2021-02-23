#include <stdio.h>

#include "include_headers.h"
#include "test_lda.h"


static void before()
{
  reset_cpu(cpu, memory);
}

static char* test_nothing_happens()
{
  //Using
  s32 cycles = 0;

  //Execute the CPU
  execute(cpu, &cycles);

  // Assert no cycles have been consumed
  mu_assert("Error, cycles were consumed", cycles == 0);
  return 0;
}

static char* test_lda_im()
{
  //Using
  memory->memory_array[0xFFFC] = INS_LDA_IM;
  memory->memory_array[0xFFFD] = 0x50;
  s32 cycles = 2;

  //Execute the cpu
  execute(cpu, &cycles);

  // Assert
  mu_assert("Accumulator loaded wrong value", cpu->Acc == 0x50);
  mu_assert("LDA_IM did not consume exactly 2 cycles", cycles == 0);
  return 0;
}

static char* test_lda_im_zeroflag()
{
  //Using
  memory->memory_array[0xFFFC] = INS_LDA_IM;
  memory->memory_array[0xFFFD] = 0x0;
  s32 cycles = 2;

  //Execute the cpu
  execute(cpu, &cycles);

  // Assert
  mu_assert("Accumulator loaded wrong value", cpu->Acc == 0x0);
  mu_assert("LDA_IM did not consume exactly 2 cycles", cycles == 0);
  mu_assert("LDA_IM did not set the zeroflag", cpu->status_flags.Z == 1);
  return 0;
}

static char* test_lda_im_negativeflag()
{
  //Using
  memory->memory_array[0xFFFC] = INS_LDA_IM;
  memory->memory_array[0xFFFD] = 0b1000000; //Bit 7 set
  s32 cycles = 2;

  //Execute the cpu
  execute(cpu, &cycles);

  // Assert
  mu_assert("Accumulator loaded wrong value", cpu->Acc == 0b1000000);
  mu_assert("LDA_IM did not consume exactly 2 cycles", cycles == 0);
  mu_assert("LDA_IM did not set the negative flag", cpu->status_flags.N == 1);
  return 0;
}

static char* all_lda_test()
{
  before();
  mu_run_test(test_lda_im);

  before();
  mu_run_test(test_nothing_happens);

  before();
  mu_run_test(test_lda_im_zeroflag);

  before();
  mu_run_test(test_lda_im_negativeflag);

  return 0;
}

int run_all_lda_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in)
{
  cpu = cpu_in;
  memory = memory_in;
  char* msg = all_lda_test();
  if(msg != 0)
  {
    printf("%s\n", msg);
    fprintf(stderr,"Not all tests passed\n");
  }
  else
  {
    printf("All LDA test passed\n");
  }

  return 0;
}