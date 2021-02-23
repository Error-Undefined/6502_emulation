
#include <stdio.h>

#include "include_headers.h"
#include "test_ldx.h"


static void before()
{
  reset_cpu(cpu, memory);
}

static char* test_ldx_im()
{
  //Given
  Byte value = 0x3f;
  s32 cycles = 2;
  memory->memory_array[0xFFFC] = INS_LDX_ABS;
  memory->memory_array[0xFFFD] = value;
  
  //Run
  execute(cpu, &cycles);

  //Expect
  mu_assert("X register was not loaded with the right value", cpu->X == value);
  mu_assert("Wrong number of cycles consumed", cycles == 0);
  return 0;
}

static char* all_ldx_test()
{
  before();
  mu_run_test(test_ldx_im);

  return 0;
}

int run_all_ldx_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in)
{
  cpu = cpu_in;
  memory = memory_in;
  
  char* msg = all_ldx_test();
  if(msg != 0)
  {
    printf("%s\n", msg);
    fprintf(stderr,"Not all LDX tests passed\n");
    return -1;
  }
  printf("All LDX test passed\n");

  return 0;
}


