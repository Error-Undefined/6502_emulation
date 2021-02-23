
#include <stdio.h>

#include "include_headers.h"
#include "test_ldx.h"


static void before()
{
  reset_cpu(cpu, memory);
}

static char* all_ldx_test()
{
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


