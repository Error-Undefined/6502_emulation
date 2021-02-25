#include <stdio.h>

#include "include_headers.h"
#include "test_jmp_call.h"

static void before()
{
  reset_cpu(cpu, memory);
}

static char* test_jmp_abs()
{
  start_test_info();

  //Given
  s32 cycles = 3;
  Byte lower_address = 0x12;
  Byte higher_address = 0x34;
  Word total_address = 0x3412;

  //Setup
  memory->memory_array[0xFFFC] = INS_JMP_ABS;
  memory->memory_array[0xFFFD] = lower_address;
  memory->memory_array[0xFFFE] = higher_address;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("JMP_ABS should take 3 cycles", cycles == 0);
  mu_assert("Program counter should have been set to the address", cpu->PC == total_address);
  return 0;
}

static char* test_jmp_ind()
{
  start_test_info();
  
  //Given
  s32 cycles = 5;
  Byte lower_indirect = 0x32;
  Byte higher_indirect = 0x54;
  Word indirect_address = 0x5432;
  
  Byte lower_jump = 0x3C;
  Byte higher_jump = 0xA2;
  Word jump_address = 0xA23C;

  // Setup
  memory->memory_array[0xFFFC] = INS_JMP_IND;
  memory->memory_array[0xFFFD] = lower_indirect;
  memory->memory_array[0xFFFE] = higher_indirect;
  memory->memory_array[indirect_address] = lower_jump;
  memory->memory_array[indirect_address + 1] = higher_jump;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("JMP_IND should take 5 cycles", cycles == 0);
  mu_assert("Program counter should have been set to the address", cpu->PC == jump_address);

  return 0;
}

static char* test_cpu_can_load_acc_after_jump()
{
  start_test_info();
  
  //Given
  s32 cycles = 5 + 2;
  Byte lower_indirect = 0x32;
  Byte higher_indirect = 0x54;
  Word indirect_address = 0x5432;
  
  Byte lower_jump = 0x3C;
  Byte higher_jump = 0xA2;
  Word jump_address = 0xA23C;

  Byte test_value = 0xFF;

  // Setup
  memory->memory_array[0xFFFC] = INS_JMP_IND;
  memory->memory_array[0xFFFD] = lower_indirect;
  memory->memory_array[0xFFFE] = higher_indirect;
  memory->memory_array[indirect_address] = lower_jump;
  memory->memory_array[indirect_address + 1] = higher_jump;

  memory->memory_array[jump_address] = INS_LDA_IM;
  memory->memory_array[jump_address + 1] = test_value;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("JMP_IND should take 5 cycles", cycles == 0);
  mu_assert("CPU could load value into accumulator after jump", cpu->Acc == test_value);

  return 0;
}

static char* test_jsr_abs()
{
  start_test_info();

  //Given
  s32 cycles = 6;
  /*
  Byte lower_address = 0x13;
  Byte higher_address = 0xC1;
  Word address_subroutine = 0xC113;
  */
  
  Byte lower_byte_in_stack = 0xFF;
  Byte higher_byte_in_stack = 0xFF;

  //Setup
  memory->memory_array[0xFFFC] = INS_JSR_ABS;
  memory->memory_array[0xFFFD] = 0x13;
  memory->memory_array[0xFFFE] = 0xC1;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("JSR_ABS should take 6 cycles", cycles == 0);
  mu_assert("Stack pointer should have decremented by two when pushing", cpu->SP == 0xFD);
  mu_assert("Stack should contain the old program counter minus one", memory->memory_array[0x1FF] == higher_byte_in_stack && memory->memory_array[0x1FE] == lower_byte_in_stack - 1);
  mu_assert("Program counter should be set to the new address", cpu->PC == 0xC113);

  return 0;
}

static char* test_rts_imp()
{
  start_test_info();
  
  //Given
  s32 cycles = 6;

  Byte lower_address_in_stack = 0x14;
  Byte higher_address_in_stack = 0x54;
  //Word address_in_stack = 0x5414;

  //Setup
  memory->memory_array[0xFFFC] = INS_RTS_IMP;
  memory->memory_array[0x1FF] = higher_address_in_stack;
  memory->memory_array[0x1FE] = lower_address_in_stack;
  cpu->SP = 0xFD;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("RTS_IMP should take 6 cycles", cycles == 0);
  mu_assert("Stack pointer should have incremented by two", cpu->SP == 0xFF);
  mu_assert("Program counter should be set to the new address", cpu->PC = 0xC113);

  return 0;
}

static char* test_jsr_and_then_rts()
{
  //Reset
  reset_cpu_word(cpu, memory, 0x8000);

  start_test_info();

  //Given
  s32 cycles = 19;
  Byte lower_address = 0x13;
  Byte higher_address = 0xC1;
  //Word address_subroutine = 0xC113;
  
  /*
  Byte lower_byte_in_stack = 0xFF;
  Byte higher_byte_in_stack = 0xFF;
  */

  //Setup
  memory->memory_array[0x8000] = INS_JSR_ABS;
  memory->memory_array[0x8001] = lower_address;
  memory->memory_array[0x8002] = higher_address;
  memory->memory_array[0x8003] = INS_LDX_IM;
  memory->memory_array[0x8004] = 0x45;

  memory->memory_array[0xC113] = INS_LDA_ABS;
  memory->memory_array[0xC114] = 0x10;
  memory->memory_array[0xC115] = INS_RTS_IMP;

  //Execute
  execute(cpu, &cycles);

  return 0;
}

static char* all_jmp_call_test()
{
  before();
  mu_run_test(test_jmp_abs);

  before();
  mu_run_test(test_jmp_ind);

  before();
  mu_run_test(test_cpu_can_load_acc_after_jump);

  before();
  mu_run_test(test_jsr_abs);

  before();
  mu_run_test(test_rts_imp);

  before();
  mu_run_test(test_jsr_and_then_rts);

  return 0;
}

int run_all_jmp_call_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in)
{
  cpu = cpu_in;
  memory = memory_in;
  char* msg = all_jmp_call_test();
  if(msg != 0)
  {
    printf("%s\n", msg);
    fprintf(stderr,"Not all jump/call tests passed\n--------\n");
    return -1;
  }

  printf("All jump/call tests passed\n---------\n");
  return 0;
}
