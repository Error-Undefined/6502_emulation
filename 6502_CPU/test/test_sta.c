
#include <stdio.h>

#include "test_sta.h"

static void before()
{
  reset_cpu(cpu, memory);
}

static char* test_sta_zp()
{
  start_test_info();

  //Given
  s32 cycles = 3;
  Byte value = 0x4;
  Byte address_zeropage = 0x45;

  //Setup
  memory->memory_array[0xFFFC] = INS_STA_ZP;
  memory->memory_array[0xFFFD] = address_zeropage;
  cpu->Acc = value;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("STA_ZP did not consume exactly 3 cycles", cycles == 0);
  mu_assert("Value was not loaded to memory", memory->memory_array[address_zeropage] == value);

  return 0;
}

static char* test_sta_zp_x()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value = 0x14;
  Byte address_zeropage = 0xA5;
  Byte x_register_offset = 0x3;

  //Setup
  memory->memory_array[0xFFFC] = INS_STA_ZP_X;
  memory->memory_array[0xFFFD] = address_zeropage;
  cpu->Acc = value;
  cpu->X = x_register_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("STA_ZP did not consume exactly 4 cycles", cycles == 0);
  mu_assert("Value was not loaded to memory", memory->memory_array[address_zeropage + x_register_offset] == value);

  return 0;
}

static char* test_sta_zp_x_wraps()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value = 0x18;
  Byte address_zeropage = 0xFE;
  Byte x_register_offset = 0x15;

  //Setup
  memory->memory_array[0xFFFC] = INS_STA_ZP_X;
  memory->memory_array[0xFFFD] = address_zeropage;
  cpu->Acc = value;
  cpu->X = x_register_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("STA_ZP did not consume exactly 4 cycles", cycles == 0);
  //0x13 as 0xFE + 0x15 = 0x113
  mu_assert("Value was not loaded to memory with zero page wrap", memory->memory_array[0x13] == value);

  return 0;
}

static char* test_sta_abs()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value = 0xAA;
  Word address = 0x1554;
  Byte lower_address = 0x54;
  Byte higher_address = 0x15;

  //Setup
  memory->memory_array[0xFFFC] = INS_STA_ABS;
  memory->memory_array[0xFFFD] = lower_address;
  memory->memory_array[0xFFFE] = higher_address;
  cpu->Acc = value;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("STA_ABS did not consume exactly 4 cycles", cycles == 0);
  mu_assert("Value was not loaded to memory", memory->memory_array[address] == value);

  return 0;
}

static char* test_sta_abs_x()
{
  start_test_info();

  //Given
  s32 cycles = 5;
  Byte value = 0x32;
  Word address = 0x1554;
  Byte lower_address = 0x54;
  Byte higher_address = 0x15;
  Byte x_offset = 0xA1;

  //Setup
  memory->memory_array[0xFFFC] = INS_STA_ABS_X;
  memory->memory_array[0xFFFD] = lower_address;
  memory->memory_array[0xFFFE] = higher_address;
  cpu->Acc = value;
  cpu->X = x_offset;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("STA_ABS_X did not consume exactly 5 cycles", cycles == 0);
  mu_assert("Value was not loaded to memory", memory->memory_array[address + x_offset] == value);

  return 0;
}

static char* test_sta_abs_y()
{
  start_test_info();

  //Given
  s32 cycles = 5;
  Byte value = 0x32;
  Word address = 0x1554;
  Byte lower_address = 0x54;
  Byte higher_address = 0x15;
  Byte y_offset = 0x21;

  //Setup
  memory->memory_array[0xFFFC] = INS_STA_ABS_Y;
  memory->memory_array[0xFFFD] = lower_address;
  memory->memory_array[0xFFFE] = higher_address;
  cpu->Acc = value;
  cpu->Y = y_offset;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("STA_ABS_Y did not consume exactly 5 cycles", cycles == 0);
  mu_assert("Value was not loaded to memory", memory->memory_array[address + y_offset] == value);

  return 0;
}

static char* test_sta_ind_x()
{
  start_test_info();

  //Given
  s32 cycles = 6;
  Byte value = 0x11;
  Byte indirect_address= 0x30;
  Byte offset_for_x_register = 0x15;
  Byte total_indirect_address = 0x45; //0x30 + 0x15
  
  Byte lower_address_for_value = 0x13;
  Byte higher_address_for_value = 0x16;
  Word address_for_value = 0x1613;

  //Setup
  memory->memory_array[0xFFFC] = INS_STA_IND_X;
  memory->memory_array[0xFFFD] = indirect_address;
  memory->memory_array[total_indirect_address] = lower_address_for_value;
  memory->memory_array[total_indirect_address + 1] = higher_address_for_value;
  cpu->Acc = value;
  cpu->X = offset_for_x_register;

  //Execute
  execute(cpu, &cycles);

  mu_assert("STA_IND_X did not consume 6 cycles", cycles == 0);
  mu_assert("Value was not loaded into memory", memory->memory_array[address_for_value] == cpu->Acc);

  return 0; 
}

static char* test_sta_ind_x_wrap()
{
  start_test_info();

  //Given
  s32 cycles = 6;
  Byte value = 0x18;
  Byte indirect_address= 0xFE;
  Byte offset_for_x_register = 0x10;
  Byte total_indirect_address = 0xE; //0xfe + 0x10 = 0x10e wraps to 0x0e
  
  Byte lower_address_for_value = 0x13;
  Byte higher_address_for_value = 0x16;
  Word address_for_value = 0x1613;

  //Setup
  memory->memory_array[0xFFFC] = INS_STA_IND_X;
  memory->memory_array[0xFFFD] = indirect_address;
  memory->memory_array[total_indirect_address] = lower_address_for_value;
  memory->memory_array[total_indirect_address + 1] = higher_address_for_value;
  cpu->Acc = value;
  cpu->X = offset_for_x_register;

  //Execute
  execute(cpu, &cycles);

  mu_assert("STA_IND_X did not consume 6 cycles", cycles == 0);
  mu_assert("Value was not loaded into memory with zp wrap", memory->memory_array[address_for_value] == value);

  return 0;

}

static char* test_sta_ind_y()
{
  start_test_info();
  
  // Given
  s32 cycles = 6;
  Byte value = 0x39;
  Byte indirect_address = 0x60;
  Byte offset_for_y_register = 0x20;

  Byte lower_address_for_value = 0xA2;
  Byte higher_address_for_value = 0x3A;

  Word address_for_value = 0x3AA2 + offset_for_y_register;

  //Setup
  memory->memory_array[0xFFFC] = INS_STA_IND_Y;
  memory->memory_array[0xFFFD] = indirect_address;
  memory->memory_array[indirect_address] = lower_address_for_value;
  memory->memory_array[indirect_address + 1] = higher_address_for_value;
  cpu->Acc = value;
  cpu->Y = offset_for_y_register;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("STA_IND_Y did not consume 6 cycles", cycles == 0);
  mu_assert("Value of accumulator was not stored into memory", value == memory->memory_array[address_for_value]);

  return 0; 
}

static char* all_sta_test()
{
  before();
  mu_run_test(test_sta_zp);

  before();
  mu_run_test(test_sta_zp_x);

  before();
  mu_run_test(test_sta_zp_x_wraps);

  before();
  mu_run_test(test_sta_abs);

  before();
  mu_run_test(test_sta_abs_x);

  before();
  mu_run_test(test_sta_abs_y);

  before();
  mu_run_test(test_sta_ind_x);

  before();
  mu_run_test(test_sta_ind_x_wrap);

  before();
  mu_run_test(test_sta_ind_y);

  return 0;
}


int run_all_sta_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in)
{
  cpu = cpu_in;
  memory = memory_in;
  
  char* msg = all_sta_test();
  if(msg != 0)
  {
    printf("%s\n", msg);
    fprintf(stderr,"Not all STA tests passed\n");
    return -1;
  }
  printf("All STA tests passed\n---------\n");

  return 0;
}
