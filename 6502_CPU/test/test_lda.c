#include <stdio.h>

#include "include_headers.h"
#include "test_lda.h"


static void before()
{
  reset_cpu(cpu, memory);
}

static void test_lda_im_setup(Byte value_to_load, s32* cycles)
{
  // Using
  memory->memory_array[0xFFFC] = INS_LDA_IM;
  memory->memory_array[0xFFFD] = value_to_load;

  // Execute
  execute(cpu, cycles);
}

static void test_lda_zp_setup(Instruction_Type instruction, Byte jump_to_zeropage, Byte value_in_zeropage, s32* cycles)
{
  //Using
  memory->memory_array[0xFFFC] = instruction;
  memory->memory_array[0xFFFD] = jump_to_zeropage;
  memory->memory_array[jump_to_zeropage + cpu->X] = value_in_zeropage;

  execute(cpu, cycles);
}

static void test_lda_abs_setup(Instruction_Type instruction, Word absolute_address, Byte value, s32* cycles)
{
  memory->memory_array[0xFFFC] = instruction;
  // Extract as little endian
  Byte lower_address = absolute_address & 0xFF;
  Byte higher_address = absolute_address >> 8;
  
  memory->memory_array[0xFFFD] = lower_address;
  memory->memory_array[0xFFFE] = higher_address;

  memory->memory_array[absolute_address + cpu->X + cpu->Y] = value;

  execute(cpu, cycles);
}

// LDA_IM

static char* test_nothing_happens()
{
  start_test_info();
  //Using
  s32 cycles = 0;

  //Execute the CPU
  execute(cpu, &cycles);

  // Assert no cycles have been consumed
  mu_assert("Error, cycles were consumed", cycles == 0);
  return 0;
}

static char* test_lda_im()
{
  start_test_info();
  //Setup and run
  s32 cycles = 2;
  test_lda_im_setup(0x50, &cycles);
  
  // Assert
  mu_assert("Accumulator loaded wrong value", cpu->Acc == 0x50);
  mu_assert("LDA_IM did not consume exactly 2 cycles", cycles == 0);
  return 0;
}

static char* test_lda_zeroflag()
{
  start_test_info();

  //Setup and run
  s32 cycles = 2;
  test_lda_im_setup(0x0, &cycles);

  // Assert
  mu_assert("Accumulator loaded wrong value", cpu->Acc == 0x0);
  mu_assert("LDA_IM did not consume exactly 2 cycles", cycles == 0);
  mu_assert("LDA_IM did not set the zeroflag", cpu->status_flags.Z == 1);
  return 0;
}

static char* test_lda_negativeflag()
{
  start_test_info();
  //Setup and run
  s32 cycles = 2;
  test_lda_im_setup(0b1000000, &cycles);

  // Assert
  mu_assert("Accumulator loaded wrong value", cpu->Acc == 0b1000000);
  mu_assert("LDA_IM did not consume exactly 2 cycles", cycles == 0);
  mu_assert("LDA_IM did not set the negative flag", cpu->status_flags.N == 1);
  return 0;
}

static char* test_lda_zp()
{
  start_test_info();

  //Setup and run
  s32 cycles = 3;
  Byte value = 0x20;
  test_lda_zp_setup(INS_LDA_ZP, 0x10, value, &cycles);

  mu_assert("Accumulator loaded wrong value", cpu->Acc == value);
  mu_assert("LDA_ZP did not consume exactly 3 cycles", cycles == 0);
  return 0;
}

static char* test_lda_zp_x()
{
  start_test_info();

  //Setup and run
  s32 cycles = 4;
  Byte value = 0x15;
  cpu->X = 0x3; // Put 3 in X register for ZP-X
  test_lda_zp_setup(INS_LDA_ZP_X, 0x10, value, &cycles);

  mu_assert("Accumulator loaded wrong value", cpu->Acc == value);
  mu_assert("LDA_ZP_X did not consume exactly 4 cycles", cycles == 0);
  
  return 0;
}

static char* test_lda_abs()
{
  start_test_info();
  
  //Setup and run
  s32 cycles = 4;
  Byte value = 0x43;
  Word absolute_address = 0x1235;
  test_lda_abs_setup(INS_LDA_ABS, absolute_address, value, &cycles);

  mu_assert("Accumulator loaded wrong value", cpu->Acc == value);
  mu_assert("LDA_ABS did not consume exactly 4 cycles", cycles == 0);
  return 0;
}

static char* test_lda_abs_x()
{
  start_test_info();

  s32 cycles = 4;
  Byte value = 0x43;
  Word absolute_address = 0x1235;
  cpu->X = 0x5;
  test_lda_abs_setup(INS_LDA_ABS_X, absolute_address, value, &cycles);

  mu_assert("Accumulator loaded wrong value", cpu->Acc == value);
  mu_assert("LDA_ABS_X did not consume exactly 4 cycles", cycles == 0);
  return 0;
}

static char* test_lda_abs_x_crosses_page()
{
  start_test_info();

  s32 cycles = 5;
  Byte value = 0x43;
  Word absolute_address = 0x12FF; // To cross a page boundary
  cpu->X = 0x5;
  test_lda_abs_setup(INS_LDA_ABS_X, absolute_address, value, &cycles);

  mu_assert("Accumulator loaded wrong value", cpu->Acc == value);
  mu_assert("LDA_ABS_X with page cross did not consume exactly 5 cycles", cycles == 0);
  return 0;
}

static char* test_lda_ind_x()
{
  start_test_info();

  //Given
  s32 cycles = 6;
  Byte value = 0xA4;
  Byte indirect_address = 0x60;
  cpu->X = 0x30;

  Byte lower_address = 0x14;
  Byte higher_address = 0x20;
  Word final_address = 0x2014;
  
  //Set up memory
  memory->memory_array[0xFFFC] = INS_LDA_IND_X;
  memory->memory_array[0xFFFD] = indirect_address;
  memory->memory_array[0x90] = lower_address;
  memory->memory_array[0x91] = higher_address;
  memory->memory_array[final_address] = value;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("Accumulator loaded wrong value", cpu->Acc == value);
  mu_assert("LDA_IND_X did not consume exactly 6 cycles", cycles == 0);
  return 0;
}

static char* test_lda_ind_x_with_wrap()
{
  start_test_info();

  //Given
  s32 cycles = 6;
  Byte value = 0xA4;
  Byte indirect_address = 0xFF;
  cpu->X = 0x30;

  Byte lower_address = 0x11;
  Byte higher_address = 0x30;
  Word final_address = 0x3011;

  //Set up memory
  memory->memory_array[0xFFFC] = INS_LDA_IND_X;
  memory->memory_array[0xFFFD] = indirect_address;
  memory->memory_array[0x2F] = lower_address; // Wrap around at the end of the zero page
  memory->memory_array[0x30] = higher_address;
  memory->memory_array[final_address] = value;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("Accumulator loaded wrong value", cpu->Acc == value);
  mu_assert("LDA_IND_X did not consume exactly 6 cycles", cycles == 0);
  return 0;
}

static char* test_lda_ind_y()
{
  start_test_info();

  //Given
  s32 cycles = 5;
  Byte value = 0xEE;
  Byte indirect_address = 0x12;
  cpu->Y = 0x15;

  Byte lower_address = 0x43;
  Byte upper_address = 0x50;
  Word final_address = 0x5043 + 0x15; //Add Y to the fetched address

  //Set up memory
  memory->memory_array[0xFFFC] = INS_LDA_IND_Y;
  memory->memory_array[0xFFFD] = indirect_address;
  memory->memory_array[0x12] = lower_address;
  memory->memory_array[0x13] = upper_address;
  memory->memory_array[final_address] = value; 

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("Accumulator loaded wrong value", cpu->Acc == value);
  mu_assert("LDA_IND_X did not consume exactly 5 cycles", cycles == 0);

  return 0;
}

static char* test_lda_ind_y_crosses_page()
{
  start_test_info();

  //Given
  s32 cycles = 6;
  Byte value = 0xEE;
  Byte indirect_address = 0x16;
  cpu->Y = 0x19;

  Byte lower_address = 0xe7;
  Byte upper_address = 0x21;

  Word final_address = 0x2200; //(Indirect address + Y register)

  //Set up memory
  memory->memory_array[0xFFFC] = INS_LDA_IND_Y;
  memory->memory_array[0xFFFD] = indirect_address;
  memory->memory_array[indirect_address] = lower_address;
  memory->memory_array[indirect_address + 1] = upper_address; 
  memory->memory_array[final_address] = value;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("Accumulator loaded wrong value", cpu->Acc == value);
  mu_assert("LDA_IND_X did not consume exactly 6 cycles", cycles == 0);
  return 0;
}

static char* all_lda_test()
{

  before();
  mu_run_test(test_nothing_happens);
  
  before();
  mu_run_test(test_lda_im);

  before();
  mu_run_test(test_lda_zeroflag);

  before();
  mu_run_test(test_lda_negativeflag);

  before();
  mu_run_test(test_lda_zp);

  before();
  mu_run_test(test_lda_zp_x);
  
  before();
  mu_run_test(test_lda_abs);

  before();
  mu_run_test(test_lda_abs_x);

  before();
  mu_run_test(test_lda_abs_x_crosses_page);

  before();
  mu_run_test(test_lda_ind_x);

  before();
  mu_run_test(test_lda_ind_x_with_wrap);

  before();
  mu_run_test(test_lda_ind_y);

  before();
  mu_run_test(test_lda_ind_y_crosses_page);

  return 0;
}

int run_all_lda_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in)
{
  cpu = cpu_in;
  memory = memory_in;
  char* msg = all_lda_test();
  if(msg != 0)
  {
    printf("%s\n", msg);
    fprintf(stderr,"Not all LDA tests passed\n--------\n");
    return -1;
  }

  printf("All LDA tests passed\n---------\n");

  return 0;
}