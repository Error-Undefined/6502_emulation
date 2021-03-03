
#include <stdio.h>

#include "include_headers.h"
#include "test_branch.h"

static void before()
{
  reset_cpu(cpu, memory);
}

static char* test_branch_carry_clear()
{
  start_test_info();

  return 0;
}

static char* test_branch_carry_set()
{
  start_test_info();

  return 0;
}

static char* test_branch_zero_clear()
{
  start_test_info();

  return 0;
}

static char* test_branch_zero_set()
{
  start_test_info();

  return 0;
}

static char* test_branch_neg_clear()
{
  start_test_info();

  return 0;
}


static char* test_branch_neg_set()
{
  start_test_info();

  return 0;
}

static char* test_branch_overflow_clear()
{
  start_test_info();

  return 0;
}

static char* test_branch_overflow_set()
{
  start_test_info();

  return 0;
}

static char* all_branch_test()
{
  before();
  mu_run_test(test_branch_carry_clear);

  before();
  mu_run_test(test_branch_carry_set);

  before();
  mu_run_test(test_branch_zero_clear);

  before();
  mu_run_test(test_branch_zero_set);

  before();
  mu_run_test(test_branch_neg_clear);

  before();
  mu_run_test(test_branch_neg_set);

  before();
  mu_run_test(test_branch_overflow_clear);

  before();
  mu_run_test(test_branch_overflow_set);

  return 0;
}


int run_all_branch_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in)
{
  cpu = cpu_in;
  memory = memory_in;
  char* msg = all_branch_test();
  if(msg != 0)
  {
    printf("%s\n", msg);
    fprintf(stderr,"Not all jump/call tests passed\n--------\n");
    return -1;
  }

  printf("All jump/call tests passed\n---------\n");
  return 0;
}
