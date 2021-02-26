
#include <stdio.h>

#include "include_headers.h"
#include "test_logic.h"


static void before()
{
  reset_cpu(cpu, memory);
}

static char* test_and_im()
{
  start_test_info();

  return 0;
}

static char* all_logic_test()
{
  before();
  mu_run_test(test_and_im);

  return 0;
}


int run_all_logic_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in)
{
  cpu = cpu_in;
  memory = memory_in;
  
  char* msg = all_logic_test();
  if(msg != 0)
  {
    printf("%s\n", msg);
    fprintf(stderr,"Not all logic tests passed\n");
    return -1;
  }
  printf("All logic tests passed\n---------\n");

  return 0;
}
