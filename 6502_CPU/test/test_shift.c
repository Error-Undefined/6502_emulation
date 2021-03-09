
#include <stdio.h>

#include "test_shift.h"

static void before()
{
  reset_cpu(cpu, memory);
}

static char* all_shift_test()
{
  before();

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

