
#include <stdio.h>

#include "include_headers.h"
#include "test_stack.h"

static void before()
{
  reset_cpu(cpu, memory);
}

static char* test_stack()
{
  start_test_info();

  return 0;
}

static char* all_stack_test()
{
  before();
  mu_run_test(test_stack);

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
