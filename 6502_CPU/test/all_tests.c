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

  printf("Running STX/STY test...\n");
  if(run_all_stx_sty_test(&cpu, &memory) != 0)
    goto test_failed;
  
  printf("Running transfers test...\n");
  if(run_all_transfers_test(&cpu, &memory) != 0)
    goto test_failed;

  printf("Running stack test...\n");
  if(run_all_stack_test(&cpu, &memory) != 0)
    goto test_failed;

  printf("Running jump/call test...\n");
  if(run_all_jmp_call_test(&cpu, &memory) != 0)
    goto test_failed;

  printf("Running logic test...\n");
  if(run_all_logic_test(&cpu, &memory) != 0)
    goto test_failed;

  printf("Running branch test...\n");
  if(run_all_branch_test(&cpu, &memory) != 0)
    goto test_failed;

  printf("Running system instruction test...\n");
  if(run_all_sys_test(&cpu, &memory) != 0)
    goto test_failed;

  printf("Running clear instruction test...\n");
  if(run_all_clear_test(&cpu, &memory) != 0)
    goto test_failed;

  printf("Running shift test...\n");
  if(run_all_shift_test(&cpu, &memory) != 0)
    goto test_failed;

  printf("End test suite, all tests passed\n");
  printf("Tests run: %d\n", tests_run);
  return 0; //All tests passed

  test_failed:
  printf("End test suite, some tests failed\n");
  printf("Tests run: %d\n", tests_run);
  return 1;

}
