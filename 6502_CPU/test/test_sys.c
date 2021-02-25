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

static char* all_sys_test()
{
  before();
  mu_run_test(test_nop);

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
