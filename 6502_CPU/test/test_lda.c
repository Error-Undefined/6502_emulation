#include <stdio.h>

#include "include_headers.h"
#include "test_lda.h"


static void before()
{
  reset_cpu(cpu, memory);
}

static void test_lda_im_setup(Byte value_to_load, s32* cycles)
{
  // Using
  memory->memory_array[0xFFFC] = INS_LDA_IM;
  memory->memory_array[0xFFFD] = value_to_load;

  // Execute
  execute(cpu, cycles);
}

static void test_lda_zp_setup(Instruction_Type instruction, Byte jump_to_zeropage, Byte value_in_zeropage, s32* cycles)
{
  //Using
  memory->memory_array[0xFFFC] = instruction;
  memory->memory_array[0xFFFD] = jump_to_zeropage;
  memory->memory_array[jump_to_zeropage] = value_in_zeropage;

  execute(cpu, cycles);
}

// LDA_IM

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
  //Setup and run
  s32 cycles = 2;
  test_lda_im_setup(0x50, &cycles);
  
  // Assert
  mu_assert("Accumulator loaded wrong value", cpu->Acc == 0x50);
  mu_assert("LDA_IM did not consume exactly 2 cycles", cycles == 0);
  return 0;
}

static char* test_lda_zeroflag()
{
  //Setup and run
  s32 cycles = 2;
  test_lda_im_setup(0x0, &cycles);

  // Assert
  mu_assert("Accumulator loaded wrong value", cpu->Acc == 0x0);
  mu_assert("LDA_IM did not consume exactly 2 cycles", cycles == 0);
  mu_assert("LDA_IM did not set the zeroflag", cpu->status_flags.Z == 1);
  return 0;
}

static char* test_lda_negativeflag()
{
  //Setup and run
  s32 cycles = 2;
  test_lda_im_setup(0b1000000, &cycles);

  // Assert
  mu_assert("Accumulator loaded wrong value", cpu->Acc == 0b1000000);
  mu_assert("LDA_IM did not consume exactly 2 cycles", cycles == 0);
  mu_assert("LDA_IM did not set the negative flag", cpu->status_flags.N == 1);
  return 0;
}

static char* test_lda_zp()
{
  //Setup and run
  s32 cycles = 3;
  Byte value = 0x20;
  test_lda_zp_setup(INS_LDA_ZP, 0x10, value, &cycles);

  mu_assert("Accumulator loaded wrong value", cpu->Acc == value);
  mu_assert("LDA_ZP did not consume exactly 3 cycles", cycles == 0);
  return 0;
}


static char* all_lda_test()
{

  before();
  mu_run_test(test_nothing_happens);
  
  before();
  mu_run_test(test_lda_im);

  before();
  mu_run_test(test_lda_zeroflag);

  before();
  mu_run_test(test_lda_negativeflag);

  before();
  mu_run_test(test_lda_zp);

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