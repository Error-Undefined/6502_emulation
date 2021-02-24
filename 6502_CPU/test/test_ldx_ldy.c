
#include <stdio.h>

#include "include_headers.h"
#include "test_ldx_ldy.h"


static void before()
{
  reset_cpu(cpu, memory);
}

static char* test_ldx_im()
{
  start_test_info();
  //Given
  Byte value = 0x3f;
  s32 cycles = 2;
  memory->memory_array[0xFFFC] = INS_LDX_IM;
  memory->memory_array[0xFFFD] = value;
  
  //Run
  execute(cpu, &cycles);

  //Expect
  mu_assert("X register was not loaded with the right value", cpu->X == value);
  mu_assert("Wrong number of cycles consumed", cycles == 0);
  
  return 0;
}

static char* test_ldx_zp()
{
  start_test_info();
  //Using
  s32 cycles = 3;
  Byte addr_in_zeropage = 0x32;
  Byte value_in_zeropage = 0x45;

  //Given
  memory->memory_array[0xFFFC] = INS_LDX_ZP;
  memory->memory_array[0xFFFD] = addr_in_zeropage;
  memory->memory_array[addr_in_zeropage] = value_in_zeropage;

  //Run
  execute(cpu, &cycles);

  //Expect
  mu_assert("X register loaded wrong value", cpu->X == value_in_zeropage);
  mu_assert("LDX_ZP did not consume exactly 3 cycles", cycles == 0);
  return 0;
}

static char* test_ldx_zp_y()
{
  start_test_info();
  //Using
  s32 cycles = 4;
  Byte addr_in_zeropage = 0x32;
  Byte value_in_zeropage = 0x45;
  cpu->Y = 0x6;

  //Given
  memory->memory_array[0xFFFC] = INS_LDX_ZP_Y;
  memory->memory_array[0xFFFD] = addr_in_zeropage;
  memory->memory_array[addr_in_zeropage + cpu->Y] = value_in_zeropage;

  //Run
  execute(cpu, &cycles);

  //Expect
  mu_assert("X register loaded wrong value", cpu->X == value_in_zeropage);
  mu_assert("LDX_ZP_Y did not consume exactly 4 cycles", cycles == 0);
  return 0;
}

static char* test_ldx_abs()
{
  start_test_info();
  //Using
  s32 cycles = 4;
  Word absolute_addr = 0x2312;
  Byte value = 0x45;

  Byte lower_address = absolute_addr & 0xFF;
  Byte higher_address = absolute_addr >> 8;

  //Given
  memory->memory_array[0xFFFC] = INS_LDX_ABS;
  memory->memory_array[0xFFFD] = lower_address;
  memory->memory_array[0xFFFE] = higher_address;
  memory->memory_array[absolute_addr] = value;

  //Run
  execute(cpu, &cycles);

  //Expect
  //Expect
  mu_assert("X register loaded wrong value", cpu->X == value);
  mu_assert("LDX_ABS did not consume exactly 4 cycles", cycles == 0);
  return 0;
}

static char* test_ldx_abs_y()
{
  start_test_info();

  //Using
  s32 cycles = 4;
  Word absolute_addr = 0x2312;
  Byte value = 0x45;
  cpu->Y = 0x32;

  Byte lower_address = absolute_addr & 0xFF;
  Byte higher_address = absolute_addr >> 8;

  //Given
  memory->memory_array[0xFFFC] = INS_LDX_ABS_Y;
  memory->memory_array[0xFFFD] = lower_address;
  memory->memory_array[0xFFFE] = higher_address;
  memory->memory_array[absolute_addr + cpu->Y] = value;

  //Run
  execute(cpu, &cycles);

  //Expect
  mu_assert("X register loaded wrong value", cpu->X == value);
  mu_assert("LDX_ABS_Y did not consume exactly 4 cycles", cycles == 0);
  return 0;
}

static char* test_ldx_abs_y_page_cross()
{
  start_test_info();
  //Using
  s32 cycles = 5;
  Word absolute_addr = 0x23FF;
  Byte value = 0x17;
  cpu->Y = 0xA1;

  Byte lower_address = absolute_addr & 0xFF;
  Byte higher_address = absolute_addr >> 8;

  //Given
  memory->memory_array[0xFFFC] = INS_LDX_ABS_Y;
  memory->memory_array[0xFFFD] = lower_address;
  memory->memory_array[0xFFFE] = higher_address;
  memory->memory_array[absolute_addr + cpu->Y] = value;

  //Run
  execute(cpu, &cycles);

  //Expect
  mu_assert("X register loaded wrong value", cpu->X == value);
  mu_assert("LDX_ABS_Y with page cross did not consume exactly 5 cycles", cycles == 0);
  return 0;
}

//---LDY tests----------------------

static char* test_ldy_im()
{
  start_test_info();
  //Given
  Byte value = 0x3f;
  s32 cycles = 2;
  memory->memory_array[0xFFFC] = INS_LDY_IM;
  memory->memory_array[0xFFFD] = value;
  
  //Run
  execute(cpu, &cycles);

  //Expect
  mu_assert("Y register was not loaded with the right value", cpu->Y == value);
  mu_assert("Wrong number of cycles consumed", cycles == 0);
  
  return 0;
}

static char* test_ldy_zp()
{
  start_test_info();
  //Using
  s32 cycles = 3;
  Byte addr_in_zeropage = 0x32;
  Byte value_in_zeropage = 0x45;

  //Given
  memory->memory_array[0xFFFC] = INS_LDY_ZP;
  memory->memory_array[0xFFFD] = addr_in_zeropage;
  memory->memory_array[addr_in_zeropage] = value_in_zeropage;

  //Run
  execute(cpu, &cycles);

  //Expect
  mu_assert("Y register loaded wrong value", cpu->Y == value_in_zeropage);
  mu_assert("LDY_ZP did not consume exactly 3 cycles", cycles == 0);
  return 0;
}

static char* test_ldy_zp_x()
{
  start_test_info();
  //Using
  s32 cycles = 4;
  Byte addr_in_zeropage = 0x32;
  Byte value_in_zeropage = 0x45;
  cpu->X = 0x13;

  //Given
  memory->memory_array[0xFFFC] = INS_LDY_ZP_X;
  memory->memory_array[0xFFFD] = addr_in_zeropage;
  memory->memory_array[addr_in_zeropage + cpu->X] = value_in_zeropage;

  //Run
  execute(cpu, &cycles);

  //Expect
  mu_assert("Y register loaded wrong value", cpu->Y == value_in_zeropage);
  mu_assert("LDY_ZP_X did not consume exactly 4 cycles", cycles == 0);
  return 0;
}

static char* test_ldy_abs()
{
  start_test_info();
  //Using
  s32 cycles = 4;
  Word absolute_addr = 0x2312;
  Byte value = 0x45;

  Byte lower_address = absolute_addr & 0xFF;
  Byte higher_address = absolute_addr >> 8;

  //Given
  memory->memory_array[0xFFFC] = INS_LDY_ABS;
  memory->memory_array[0xFFFD] = lower_address;
  memory->memory_array[0xFFFE] = higher_address;
  memory->memory_array[absolute_addr] = value;

  //Run
  execute(cpu, &cycles);

  //Expect
  //Expect
  mu_assert("Y register loaded wrong value", cpu->Y == value);
  mu_assert("LDY_ABS did not consume exactly 4 cycles", cycles == 0);
  return 0;
}

static char* test_ldy_abs_x()
{
  start_test_info();

  //Using
  s32 cycles = 4;
  Word absolute_addr = 0x2312;
  Byte value = 0x45;
  cpu->X = 0x32;

  Byte lower_address = absolute_addr & 0xFF;
  Byte higher_address = absolute_addr >> 8;

  //Given
  memory->memory_array[0xFFFC] = INS_LDY_ABS_X;
  memory->memory_array[0xFFFD] = lower_address;
  memory->memory_array[0xFFFE] = higher_address;
  memory->memory_array[absolute_addr + cpu->X] = value;

  //Run
  execute(cpu, &cycles);

  //Expect
  mu_assert("Y register loaded wrong value", cpu->Y == value);
  mu_assert("LDY_ABS_X did not consume exactly 4 cycles", cycles == 0);
  return 0;
}

static char* test_ldy_abs_x_page_cross()
{
  start_test_info();
  //Using
  s32 cycles = 5;
  Word absolute_addr = 0x23FF;
  Byte value = 0x17;
  cpu->X = 0xA2;

  Byte lower_address = absolute_addr & 0xFF;
  Byte higher_address = absolute_addr >> 8;

  //Given
  memory->memory_array[0xFFFC] = INS_LDY_ABS_X;
  memory->memory_array[0xFFFD] = lower_address;
  memory->memory_array[0xFFFE] = higher_address;
  memory->memory_array[absolute_addr + cpu->X] = value;

  //Run
  execute(cpu, &cycles);

  //Expect
  mu_assert("Y register loaded wrong value", cpu->Y == value);
  mu_assert("LDY_ABS_X with page cross did not consume exactly 5 cycles", cycles == 0);
  return 0;
}

static char* all_ldx_test()
{
  //LDX tests
  before();
  mu_run_test(test_ldx_im);

  before();
  mu_run_test(test_ldx_zp);

  before();
  mu_run_test(test_ldx_zp_y);
  
  before();
  mu_run_test(test_ldx_abs);

  before();
  mu_run_test(test_ldx_abs_y);

  before();
  mu_run_test(test_ldx_abs_y_page_cross);

  //LDY tests
  before();
  mu_run_test(test_ldy_im);

  before();
  mu_run_test(test_ldy_zp);

  before();
  mu_run_test(test_ldy_zp_x);
  
  before();
  mu_run_test(test_ldy_abs);

  before();
  mu_run_test(test_ldy_abs_x);

  before();
  mu_run_test(test_ldy_abs_x_page_cross);

  return 0;
}

int run_all_ldx_ldy_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in)
{
  cpu = cpu_in;
  memory = memory_in;
  
  char* msg = all_ldx_test();
  if(msg != 0)
  {
    printf("%s\n", msg);
    fprintf(stderr,"Not all LDX/LDY tests passed\n");
    return -1;
  }
  printf("All LDX/LDY tests passed\n---------\n");

  return 0;
}


