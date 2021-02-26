
#include <stdio.h>

#include "include_headers.h"
#include "test_logic.h"


static void before()
{
  reset_cpu(cpu, memory);
}

/* AND tests */
static char* test_and_im()
{
  start_test_info();

  //Given
  s32 cycles = 2;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc & value_mem;

  //Setup
  memory->memory_array[0xFFFC] = INS_AND_IM;
  memory->memory_array[0xFFFD] = value_mem;
  cpu->Acc = value_acc;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_AND_IM should take 2 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been ANDed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_and_zp()
{
  start_test_info();

  //Given
  s32 cycles = 3;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc & value_mem;
  Byte addr_zp = 0x50;

  //Setup
  memory->memory_array[0xFFFC] = INS_AND_ZP;
  memory->memory_array[0xFFFD] = addr_zp;
  memory->memory_array[addr_zp] = value_mem;
  cpu->Acc = value_acc;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_AND_ZP should take 3 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been ANDed with value", cpu->Acc == expected_result);

  return 0;
}

static char* test_and_zp_x()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc & value_mem;
  Byte addr_zp = 0x50;
  Byte zp_x_offset = 0x10;

  //Setup
  memory->memory_array[0xFFFC] = INS_AND_ZP_X;
  memory->memory_array[0xFFFD] = addr_zp;
  memory->memory_array[addr_zp + zp_x_offset] = value_mem;
  cpu->Acc = value_acc;
  cpu->X = zp_x_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_AND_ZP_X should take 4 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been ANDed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_and_abs()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc & value_mem;
  Byte lower_mem_address = 0x42;
  Byte higher_mem_address = 0x3C;
  Word total_mem_address = 0x3C42;

  //Setup
  memory->memory_array[0xFFFC] = INS_AND_ABS;
  memory->memory_array[0xFFFD] = lower_mem_address;
  memory->memory_array[0xFFFE] = higher_mem_address;
  memory->memory_array[total_mem_address] = value_mem;
  cpu->Acc = value_acc;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_AND_ABS should take 4 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been ANDed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_and_abs_x()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc & value_mem;
  Byte lower_mem_address = 0x42;
  Byte higher_mem_address = 0x3C;
  Word total_mem_address = 0x3C42;
  Byte x_offset = 0x13;

  //Setup
  memory->memory_array[0xFFFC] = INS_AND_ABS_X;
  memory->memory_array[0xFFFD] = lower_mem_address;
  memory->memory_array[0xFFFE] = higher_mem_address;
  memory->memory_array[total_mem_address + x_offset] = value_mem;
  cpu->Acc = value_acc;
  cpu->X = x_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_AND_ABS_X should take 4 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been ANDed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_and_abs_y()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc & value_mem;
  Byte lower_mem_address = 0x42;
  Byte higher_mem_address = 0x3C;
  Word total_mem_address = 0x3C42;
  Byte y_offset = 0x27;

  //Setup
  memory->memory_array[0xFFFC] = INS_AND_ABS_Y;
  memory->memory_array[0xFFFD] = lower_mem_address;
  memory->memory_array[0xFFFE] = higher_mem_address;
  memory->memory_array[total_mem_address + y_offset] = value_mem;
  cpu->Acc = value_acc;
  cpu->Y = y_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_AND_ABS_Y should take 4 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been ANDed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_and_ind_x()
{
  start_test_info();

  //Given
  s32 cycles = 6;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc & value_mem;
  Byte ind_addr = 0xEF;
  Byte x_offset = 0x52; // Wraps to 0x41
  Byte lower_address = 0xC1;
  Byte higher_address = 0xA5;
  Word final_address = 0xA5C1;

  //Setup
  memory->memory_array[0xFFFC] = INS_AND_IND_X;
  memory->memory_array[0xFFFD] = ind_addr;
  memory->memory_array[0x41] = lower_address;
  memory->memory_array[0x42] = higher_address;
  memory->memory_array[final_address] = value_mem;
  cpu->Acc = value_acc;
  cpu->X = x_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_AND_IND_X should take 6 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been ANDed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_and_ind_y()
{
  start_test_info();

  //Given
  s32 cycles = 6;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc & value_mem;
  Byte ind_addr = 0xEF;
  Byte y_offset = 0xA3;
  Byte lower_address = 0xF1;
  Byte higher_address = 0x47;
  Word final_address = 0x47F1 + y_offset;

  //Setup
  memory->memory_array[0xFFFC] = INS_AND_IND_Y;
  memory->memory_array[0xFFFD] = ind_addr;
  memory->memory_array[0xEF] = lower_address;
  memory->memory_array[0xF0] = higher_address;
  memory->memory_array[final_address] = value_mem;
  cpu->Acc = value_acc;
  cpu->Y = y_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_AND_IND_Y should take 6 clock cycles (page cross)", cycles == 0);
  mu_assert("Contents of accumulator should have been ANDed with value", cpu->Acc == expected_result);

  return 0;
}

/* EOR tests */

static char* test_eor_im()
{
  start_test_info();

  //Given
  s32 cycles = 2;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc ^ value_mem;

  //Setup
  memory->memory_array[0xFFFC] = INS_EOR_IM;
  memory->memory_array[0xFFFD] = value_mem;
  cpu->Acc = value_acc;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_EOR_IM should take 2 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been EOR with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_eor_zp()
{
  start_test_info();

  //Given
  s32 cycles = 3;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc ^ value_mem;
  Byte addr_zp = 0x50;

  //Setup
  memory->memory_array[0xFFFC] = INS_EOR_ZP;
  memory->memory_array[0xFFFD] = addr_zp;
  memory->memory_array[addr_zp] = value_mem;
  cpu->Acc = value_acc;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_EOR_ZP should take 3 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been EORed with value", cpu->Acc == expected_result);

  return 0;
}

static char* test_eor_zp_x()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc ^ value_mem;
  Byte addr_zp = 0x50;
  Byte zp_x_offset = 0x10;

  //Setup
  memory->memory_array[0xFFFC] = INS_EOR_ZP_X;
  memory->memory_array[0xFFFD] = addr_zp;
  memory->memory_array[addr_zp + zp_x_offset] = value_mem;
  cpu->Acc = value_acc;
  cpu->X = zp_x_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_EOR_ZP_X should take 4 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been EORed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_eor_abs()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc ^ value_mem;
  Byte lower_mem_address = 0x42;
  Byte higher_mem_address = 0x3C;
  Word total_mem_address = 0x3C42;

  //Setup
  memory->memory_array[0xFFFC] = INS_EOR_ABS;
  memory->memory_array[0xFFFD] = lower_mem_address;
  memory->memory_array[0xFFFE] = higher_mem_address;
  memory->memory_array[total_mem_address] = value_mem;
  cpu->Acc = value_acc;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_EOR_ABS should take 4 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been EORed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_eor_abs_x()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc ^ value_mem;
  Byte lower_mem_address = 0x42;
  Byte higher_mem_address = 0x3C;
  Word total_mem_address = 0x3C42;
  Byte x_offset = 0x13;

  //Setup
  memory->memory_array[0xFFFC] = INS_EOR_ABS_X;
  memory->memory_array[0xFFFD] = lower_mem_address;
  memory->memory_array[0xFFFE] = higher_mem_address;
  memory->memory_array[total_mem_address + x_offset] = value_mem;
  cpu->Acc = value_acc;
  cpu->X = x_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_EOR_ABS_X should take 4 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been EORed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_eor_abs_y()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc ^ value_mem;
  Byte lower_mem_address = 0x42;
  Byte higher_mem_address = 0x3C;
  Word total_mem_address = 0x3C42;
  Byte y_offset = 0x27;

  //Setup
  memory->memory_array[0xFFFC] = INS_EOR_ABS_Y;
  memory->memory_array[0xFFFD] = lower_mem_address;
  memory->memory_array[0xFFFE] = higher_mem_address;
  memory->memory_array[total_mem_address + y_offset] = value_mem;
  cpu->Acc = value_acc;
  cpu->Y = y_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_EOR_ABS_Y should take 4 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been EORed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_eor_ind_x()
{
  start_test_info();

  //Given
  s32 cycles = 6;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc ^ value_mem;
  Byte ind_addr = 0xEF;
  Byte x_offset = 0x52; // Wraps to 0x41
  Byte lower_address = 0xC1;
  Byte higher_address = 0xA5;
  Word final_address = 0xA5C1;

  //Setup
  memory->memory_array[0xFFFC] = INS_EOR_IND_X;
  memory->memory_array[0xFFFD] = ind_addr;
  memory->memory_array[0x41] = lower_address;
  memory->memory_array[0x42] = higher_address;
  memory->memory_array[final_address] = value_mem;
  cpu->Acc = value_acc;
  cpu->X = x_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_EOR_IND_X should take 6 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been EORed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_eor_ind_y()
{
  start_test_info();

  //Given
  s32 cycles = 6;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc ^ value_mem;
  Byte ind_addr = 0xEF;
  Byte y_offset = 0xA3;
  Byte lower_address = 0xF1;
  Byte higher_address = 0x47;
  Word final_address = 0x47F1 + y_offset;

  //Setup
  memory->memory_array[0xFFFC] = INS_EOR_IND_Y;
  memory->memory_array[0xFFFD] = ind_addr;
  memory->memory_array[0xEF] = lower_address;
  memory->memory_array[0xF0] = higher_address;
  memory->memory_array[final_address] = value_mem;
  cpu->Acc = value_acc;
  cpu->Y = y_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_EOR_IND_Y should take 6 clock cycles (page cross)", cycles == 0);
  mu_assert("Contents of accumulator should have been EORed with value", cpu->Acc == expected_result);

  return 0;
}

/* ORA tests */

static char* test_ora_im()
{
  start_test_info();

  //Given
  s32 cycles = 2;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc | value_mem;

  //Setup
  memory->memory_array[0xFFFC] = INS_ORA_IM;
  memory->memory_array[0xFFFD] = value_mem;
  cpu->Acc = value_acc;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_ORA_IM should take 2 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been ORA with value", cpu->Acc == expected_result);
  mu_assert("Program counter should increment", cpu->PC == 0xFFFE);

  return 0;
}


static char* test_ora_zp()
{
  start_test_info();

  //Given
  s32 cycles = 3;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc | value_mem;
  Byte addr_zp = 0x50;

  //Setup
  memory->memory_array[0xFFFC] = INS_ORA_ZP;
  memory->memory_array[0xFFFD] = addr_zp;
  memory->memory_array[addr_zp] = value_mem;
  cpu->Acc = value_acc;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_ORA_ZP should take 3 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been ORAed with value", cpu->Acc == expected_result);

  return 0;
}

static char* test_ora_zp_x()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc | value_mem;
  Byte addr_zp = 0x50;
  Byte zp_x_offset = 0x10;

  //Setup
  memory->memory_array[0xFFFC] = INS_ORA_ZP_X;
  memory->memory_array[0xFFFD] = addr_zp;
  memory->memory_array[addr_zp + zp_x_offset] = value_mem;
  cpu->Acc = value_acc;
  cpu->X = zp_x_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_ORA_ZP_X should take 4 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been ORAed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_ora_abs()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc | value_mem;
  Byte lower_mem_address = 0x42;
  Byte higher_mem_address = 0x3C;
  Word total_mem_address = 0x3C42;

  //Setup
  memory->memory_array[0xFFFC] = INS_ORA_ABS;
  memory->memory_array[0xFFFD] = lower_mem_address;
  memory->memory_array[0xFFFE] = higher_mem_address;
  memory->memory_array[total_mem_address] = value_mem;
  cpu->Acc = value_acc;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_ORA_ABS should take 4 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been ORAed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_ora_abs_x()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc | value_mem;
  Byte lower_mem_address = 0x42;
  Byte higher_mem_address = 0x3C;
  Word total_mem_address = 0x3C42;
  Byte x_offset = 0x13;

  //Setup
  memory->memory_array[0xFFFC] = INS_ORA_ABS_X;
  memory->memory_array[0xFFFD] = lower_mem_address;
  memory->memory_array[0xFFFE] = higher_mem_address;
  memory->memory_array[total_mem_address + x_offset] = value_mem;
  cpu->Acc = value_acc;
  cpu->X = x_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_ORA_ABS_X should take 4 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been ORAed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_ora_abs_y()
{
  start_test_info();

  //Given
  s32 cycles = 4;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc | value_mem;
  Byte lower_mem_address = 0x42;
  Byte higher_mem_address = 0x3C;
  Word total_mem_address = 0x3C42;
  Byte y_offset = 0x27;

  //Setup
  memory->memory_array[0xFFFC] = INS_ORA_ABS_Y;
  memory->memory_array[0xFFFD] = lower_mem_address;
  memory->memory_array[0xFFFE] = higher_mem_address;
  memory->memory_array[total_mem_address + y_offset] = value_mem;
  cpu->Acc = value_acc;
  cpu->Y = y_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_ORA_ABS_Y should take 4 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been ORAed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_ora_ind_x()
{
  start_test_info();

  //Given
  s32 cycles = 6;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc | value_mem;
  Byte ind_addr = 0xEF;
  Byte x_offset = 0x52; // Wraps to 0x41
  Byte lower_address = 0xC1;
  Byte higher_address = 0xA5;
  Word final_address = 0xA5C1;

  //Setup
  memory->memory_array[0xFFFC] = INS_ORA_IND_X;
  memory->memory_array[0xFFFD] = ind_addr;
  memory->memory_array[0x41] = lower_address;
  memory->memory_array[0x42] = higher_address;
  memory->memory_array[final_address] = value_mem;
  cpu->Acc = value_acc;
  cpu->X = x_offset;

  //Execute
  execute(cpu, &cycles);
  //Assert
  mu_assert("INS_ORA_IND_X should take 6 clock cycles", cycles == 0);
  mu_assert("Contents of accumulator should have been ORAed with value", cpu->Acc == expected_result);

  return 0;
}


static char* test_ora_ind_y()
{
  start_test_info();

  //Given
  s32 cycles = 6;
  Byte value_acc = 0xC2;
  Byte value_mem = 0xA5;
  Byte expected_result = value_acc | value_mem;
  Byte ind_addr = 0xEF;
  Byte y_offset = 0xA3;
  Byte lower_address = 0xF1;
  Byte higher_address = 0x47;
  Word final_address = 0x47F1 + y_offset;

  //Setup
  memory->memory_array[0xFFFC] = INS_ORA_IND_Y;
  memory->memory_array[0xFFFD] = ind_addr;
  memory->memory_array[0xEF] = lower_address;
  memory->memory_array[0xF0] = higher_address;
  memory->memory_array[final_address] = value_mem;
  cpu->Acc = value_acc;
  cpu->Y = y_offset;

  //Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_ORA_IND_Y should take 6 clock cycles (page cross)", cycles == 0);
  mu_assert("Contents of accumulator should have been ORAed with value", cpu->Acc == expected_result);

  return 0;
}

/* BIT test */

static char* test_bit_zp()
{
  start_test_info();
  // Given
  s32 cycles = 3;
  Byte value_mem = 0xA4;
  Byte addr_zp = 0x31;
  Byte acc_value = 0x53; // Chosen as 0xA4 & 0x53 = 0

  // Setup
  memory->memory_array[0xFFFC] = INS_BIT_ZP;
  memory->memory_array[0xFFFD] = addr_zp;
  memory->memory_array[addr_zp] = value_mem;
  cpu->Acc = acc_value;
  cpu->status_flags.V = 0;
  cpu->status_flags.N = 0;
  cpu->status_flags.Z = 0;

  // Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_BIT_ZP should take 3 cycles", cycles == 0);
  mu_assert("V status flag should be set correctly", cpu->status_flags.V == 0);
  mu_assert("N status flag should be set correctly", cpu->status_flags.N == 1);
  mu_assert("When AND op returns 0, zero flag should be set", cpu->status_flags.Z == 1);

  return 0;
}

static char* test_bit_abs()
{
   start_test_info();
  // Given
  s32 cycles = 4;
  Byte value_mem = 0x72;
  Byte lower_address = 0x41;
  Byte higher_Address = 0xAA;
  Word total_address = 0xAA41;
  Byte acc_value = 0x19; // Chosen as 0xA4 & 0x53 != 0

  // Setup
  memory->memory_array[0xFFFC] = INS_BIT_ZP;
  memory->memory_array[0xFFFD] = lower_address;
  memory->memory_array[0xFFFE] = higher_Address;
  memory->memory_array[total_address] = value_mem;
  cpu->Acc = acc_value;
  cpu->status_flags.V = 0;
  cpu->status_flags.N = 0;
  cpu->status_flags.Z = 0;

  // Execute
  execute(cpu, &cycles);

  //Assert
  mu_assert("INS_BIT_ABS should take 4 cycles", cycles == 0);
  mu_assert("V status flag should be set correctly", cpu->status_flags.V == 1);
  mu_assert("N status flag should be set correctly", cpu->status_flags.N == 0);
  mu_assert("When AND op is not 0, zero flag should not be set", cpu->status_flags.Z == 0);

  return 0;
}

static char* all_logic_test()
{
  before();
  mu_run_test(test_and_im);

  before();
  mu_run_test(test_and_zp);

  before();
  mu_run_test(test_and_zp_x);

  before();
  mu_run_test(test_and_abs);

  before();
  mu_run_test(test_and_abs_x);

  before();
  mu_run_test(test_and_abs_y);

  before();
  mu_run_test(test_and_ind_x);

  before(); 
  mu_run_test(test_and_ind_y);


  before();
  mu_run_test(test_eor_im);

  before();
  mu_run_test(test_eor_zp);

  before();
  mu_run_test(test_eor_zp_x);

  before();
  mu_run_test(test_eor_abs);

  before();
  mu_run_test(test_eor_abs_x);

  before();
  mu_run_test(test_eor_abs_y);

  before();
  mu_run_test(test_eor_ind_x);

  before(); 
  mu_run_test(test_eor_ind_y);

  before();
  mu_run_test(test_ora_im);

  before();
  mu_run_test(test_ora_zp);

  before();
  mu_run_test(test_ora_zp_x);

  before();
  mu_run_test(test_ora_abs);

  before();
  mu_run_test(test_ora_abs_x);

  before();
  mu_run_test(test_ora_abs_y);

  before();
  mu_run_test(test_ora_ind_x);

  before(); 
  mu_run_test(test_ora_ind_y);

  before(); 
  mu_run_test(test_bit_zp);

  before(); 
  mu_run_test(test_bit_abs);

  return 0;
}


int run_all_logic_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in)
{
  cpu = cpu_in;
  memory = memory_in;
  
  char* msg = all_logic_test();
  if(msg != 0)
  {
    printf("%s\n", msg);
    fprintf(stderr,"Not all logic tests passed\n");
    return -1;
  }
  printf("All logic tests passed\n---------\n");

  return 0;
}
