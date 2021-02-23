#include <stdio.h>

#include "all_tests.h"

int tests_run = 0;

int main()
{
  struct cpu_struct cpu;
  struct memory_struct memory;

  printf("Running LDA test...\n");
  run_all_lda_test(&cpu, &memory);

  printf("Running LDX test...\n");
  run_all_ldx_test(&cpu, &memory);
  

  printf("End test suite\n");

  printf("Tests run: %d\n", tests_run);
}
