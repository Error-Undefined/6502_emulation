
#include <stdio.h>

#include "include_headers.h"
#include "test_branch.h"

static void before()
{
  // Always reset to 0x8080
  reset_cpu_word(cpu, memory, 0x8080);
}

static char* test_branch_carry_clear()
{
  start_test_info();
  //Given
  s32 cycles = 2 + 1;
  Byte branch_offset = 0x64; //0xFF signed is equivalent to -1 ASSUMING TWO'S COMPLEMENT

  //Setup
  memory->memory_array[0x8080] = INS_BCC_REL;
  memory->memory_array[0x8081] = branch_offset;
  cpu->status_flags.C = 0;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Successful branch should take 3 cycles", cycles == 0);
  mu_assert("Program counter not set to the right value after branching", cpu->PC == 0x8082 + (RelativeOffset) branch_offset);

  return 0;
}

static char* test_branch_carry_set()
{
  start_test_info();
  
  //Given
  s32 cycles = 2 + 1;
  Byte branch_offset = 0x64; //0xFF signed is equivalent to -1 ASSUMING TWO'S COMPLEMENT

  //Setup
  memory->memory_array[0x8080] = INS_BCS_REL;
  memory->memory_array[0x8081] = branch_offset;
  cpu->status_flags.C = 1;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Successful branch should take 3 cycles", cycles == 0);
  mu_assert("Program counter not set to the right value after branching", cpu->PC == 0x8082 + (RelativeOffset) branch_offset);

  return 0;
}

static char* test_branch_zero_clear()
{
  start_test_info();

  //Given
  s32 cycles = 2 + 1;
  Byte branch_offset = 0x64; //0xFF signed is equivalent to -1 ASSUMING TWO'S COMPLEMENT

  //Setup
  memory->memory_array[0x8080] = INS_BNE_REL;
  memory->memory_array[0x8081] = branch_offset;
  cpu->status_flags.Z = 0;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Successful branch should take 3 cycles", cycles == 0);
  mu_assert("Program counter not set to the right value after branching", cpu->PC == 0x8082 + (RelativeOffset) branch_offset);

  return 0;
}

static char* test_branch_zero_set()
{
  start_test_info();

  //Given
  s32 cycles = 2 + 1;
  Byte branch_offset = 0x64; //0xFF signed is equivalent to -1 ASSUMING TWO'S COMPLEMENT

  //Setup
  memory->memory_array[0x8080] = INS_BEQ_REL;
  memory->memory_array[0x8081] = branch_offset;
  cpu->status_flags.Z = 1;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Successful branch should take 3 cycles", cycles == 0);
  mu_assert("Program counter not set to the right value after branching", cpu->PC == 0x8082 + (RelativeOffset) branch_offset);

  return 0;
}

static char* test_branch_neg_clear()
{
  start_test_info();

  //Given
  s32 cycles = 2 + 1;
  Byte branch_offset = 0x64; //0xFF signed is equivalent to -1 ASSUMING TWO'S COMPLEMENT

  //Setup
  memory->memory_array[0x8080] = INS_BPL_REL;
  memory->memory_array[0x8081] = branch_offset;
  cpu->status_flags.N = 0;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Successful branch should take 3 cycles", cycles == 0);
  mu_assert("Program counter not set to the right value after branching", cpu->PC == 0x8082 + (RelativeOffset) branch_offset);

  return 0;
}


static char* test_branch_neg_set()
{
  start_test_info();

  //Given
  s32 cycles = 2 + 1;
  Byte branch_offset = 0x64; //0xFF signed is equivalent to -1 ASSUMING TWO'S COMPLEMENT

  //Setup
  memory->memory_array[0x8080] = INS_BMI_REL;
  memory->memory_array[0x8081] = branch_offset;
  cpu->status_flags.N = 1;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Successful branch should take 3 cycles", cycles == 0);
  mu_assert("Program counter not set to the right value after branching", cpu->PC == 0x8082 + (RelativeOffset) branch_offset);

  return 0;
}

static char* test_branch_overflow_clear()
{
  start_test_info();

  //Given
  s32 cycles = 2 + 1;
  Byte branch_offset = 0x64; //0xFF signed is equivalent to -1 ASSUMING TWO'S COMPLEMENT

  //Setup
  memory->memory_array[0x8080] = INS_BVC_REL;
  memory->memory_array[0x8081] = branch_offset;
  cpu->status_flags.V = 0;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Successful branch should take 3 cycles", cycles == 0);
  mu_assert("Program counter not set to the right value after branching", cpu->PC == 0x8082 + (RelativeOffset) branch_offset);

  return 0;
}

static char* test_branch_overflow_set()
{
  start_test_info();

  //Given
  s32 cycles = 2 + 1;
  Byte branch_offset = 0x64; //0xFF signed is equivalent to -1 ASSUMING TWO'S COMPLEMENT

  //Setup
  memory->memory_array[0x8080] = INS_BVS_REL;
  memory->memory_array[0x8081] = branch_offset;
  cpu->status_flags.V = 1;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("Successful branch should take 3 cycles", cycles == 0);
  mu_assert("Program counter not set to the right value after branching", cpu->PC == 0x8082 + (RelativeOffset) branch_offset);

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
    fprintf(stderr,"Not all branch tests passed\n--------\n");
    return -1;
  }

  printf("All branch tests passed\n---------\n");
  return 0;
}
