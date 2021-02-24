#include <stdio.h>

#include "all_tests.h"

int tests_run = 0;

int main()
{
  struct cpu_struct cpu;
  struct memory_struct memory;

  printf("Running LDA test...\n");
  if(run_all_lda_test(&cpu, &memory) != 0) 
    goto test_failed;

  printf("Running LDX/LDY test...\n");
  if(run_all_ldx_ldy_test(&cpu, &memory) != 0)
    goto test_failed;
  
  printf("Running STA test...\n");
  if(run_all_sta_test(&cpu, &memory) != 0)
    goto test_failed;

  printf("Running system instruction test...\n");
  if(run_all_sys_test(&cpu, &memory) != 0)
    goto test_failed;

  printf("End test suite, all tests passed\n");
  printf("Tests run: %d\n", tests_run);
  return 0; //All tests passed

  test_failed:
  printf("End test suite, some tests failed\n");
  printf("Tests run: %d\n", tests_run);
  return 1;

}
