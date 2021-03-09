
#include <stdio.h>

#include "test_clear.h"

static char* all_clear_test()
{
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
    fprintf(stderr,"Not all branch tests passed\n--------\n");
    return -1;
  }

  printf("All branch tests passed\n---------\n");
  return 0;
}
