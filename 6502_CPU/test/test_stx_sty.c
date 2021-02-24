#include <stdio.h>

#include "include_headers.h"
#include "test_stx_sty.h"

static void before()
{
  reset_cpu(cpu, memory);
}

static char* test_stx_zp()
{
  start_test_info();
  
  //Given
  s32 cycles = 3;
  Byte value = 0x11;
  Byte zeropage_address = 0x50;

  //Setup
  memory->memory_array[0xFFFC] = INS_STX_ZP;
  memory->memory_array[0xFFFD] = zeropage_address;
  cpu->X = value;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("STX_ZP should consume 3 cycles", cycles == 0);
  mu_assert("X register should be put into memory", memory->memory_array[zeropage_address] == value);

  return 0;
}

static char* test_stx_zp_y()
{
  start_test_info();
  
  //Given
  s32 cycles = 4;
  Byte value = 0x13;
  Byte zeropage_address = 0xB3;
  Byte y_register_offset = 0x11;
  Byte final_address = zeropage_address + y_register_offset;

  //Setup
  memory->memory_array[0xFFFC] = INS_STX_ZP_Y;
  memory->memory_array[0xFFFD] = zeropage_address;
  cpu->X = value;
  cpu->Y = y_register_offset;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("STY_ZP should consume 4 cycles", cycles == 0);
  mu_assert("X register should be put into memory", memory->memory_array[final_address] == value);

  return 0;
}

static char* test_stx_abs()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value = 0x71;
  Byte lower_absolute_address = 0xAE;
  Byte higher_absolute_address = 0x12;
  Word absolute_address = 0x12AE;

  //Setup
  memory->memory_array[0xFFFC] = INS_STX_ABS;
  memory->memory_array[0xFFFD] = lower_absolute_address;
  memory->memory_array[0xFFFE] = higher_absolute_address;
  cpu->X = value;

  //execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("STX_ABS should consume 4 cycles", cycles == 0);
  mu_assert("X register should be put into memory", memory->memory_array[absolute_address] == value);

  return 0;
}

static char* test_sty_zp()
{
  start_test_info();
  
  //Given
  s32 cycles = 3;
  Byte value = 0x39;
  Byte zeropage_address = 0x2A;

  //Setup
  memory->memory_array[0xFFFC] = INS_STX_ZP;
  memory->memory_array[0xFFFD] = zeropage_address;
  cpu->Y = value;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("STY_ZP should consume 3 cycles", cycles == 0);
  mu_assert("Y register should be put into memory", memory->memory_array[zeropage_address] == value);

  return 0;
}

static char* test_sty_zp_x()
{
  start_test_info();
  
  //Given
  s32 cycles = 4;
  Byte value = 0xF7;
  Byte zeropage_address = 0xA3;
  Byte x_register_offset = 0x17;
  Byte final_address = zeropage_address + x_register_offset;

  //Setup
  memory->memory_array[0xFFFC] = INS_STY_ZP_X;
  memory->memory_array[0xFFFD] = zeropage_address;
  cpu->Y = value;
  cpu->X = x_register_offset;

  //Execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("STY_ZP_X should consume 4 cycles", cycles == 0);
  mu_assert("Y register should be put into memory", memory->memory_array[final_address] == value);

  return 0;
}

static char* test_sty_abs()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value = 0x7;
  Byte lower_absolute_address = 0xC7;
  Byte higher_absolute_address = 0x1E;
  Word absolute_address = 0x1EC7;

  //Setup
  memory->memory_array[0xFFFC] = INS_STY_ABS;
  memory->memory_array[0xFFFD] = lower_absolute_address;
  memory->memory_array[0xFFFE] = higher_absolute_address;
  cpu->Y = value;

  //execute
  execute(cpu, &cycles);

  //Expect
  mu_assert("STY_ABS should consume 4 cycles", cycles == 0);
  mu_assert("Y register should be put into memory", memory->memory_array[absolute_address] == value);

  return 0;
}

static char* all_stx_sty_test()
{
  before();
  mu_run_test(test_stx_zp);

  before();
  mu_run_test(test_stx_zp_y);

  before();
  mu_run_test(test_stx_abs);

  before();
  mu_run_test(test_sty_zp);

  before();
  mu_run_test(test_sty_zp_x);

  before();
  mu_run_test(test_sty_abs);

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