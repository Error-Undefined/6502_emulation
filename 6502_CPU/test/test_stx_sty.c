#include <stdio.h>

#include "include_headers.h"
#include "test_stx_sty.h"

static void before()
{
  reset_cpu(cpu, memory);
}

static char* all_stx_sty_test()
{
  before();


  return 0;
}

int run_all_stx_sty_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in)
{
  cpu = cpu_in;
  memory = memory_in;
  char* msg = all_stx_sty_test();
  if(msg != 0)
  {
    printf("%s\n", msg);
    fprintf(stderr,"Not all STX/STY tests passed\n--------\n");
    return -1;
  }

  printf("All STX/STY tests passed\n---------\n");
  return 0;
}