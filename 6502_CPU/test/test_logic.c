
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
  Byte value_acc = 0xc2;
  Byte value_mem = 0xa5;
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

  return 0;
}

static char* test_and_zp_x()
{
  start_test_info();

  return 0;
}


static char* test_and_abs()
{
  start_test_info();

  return 0;
}


static char* test_and_abs_x()
{
  start_test_info();

  return 0;
}


static char* test_and_abs_y()
{
  start_test_info();

  return 0;
}


static char* test_and_ind_x()
{
  start_test_info();

  return 0;
}


static char* test_and_ind_y()
{
  start_test_info();

  return 0;
}

/* EOR tests */

static char* test_eor_im()
{
  start_test_info();

  return 0;
}


static char* test_eor_zp()
{
  start_test_info();

  return 0;
}

static char* test_eor_zp_x()
{
  start_test_info();

  return 0;
}


static char* test_eor_abs()
{
  start_test_info();

  return 0;
}


static char* test_eor_abs_x()
{
  start_test_info();

  return 0;
}


static char* test_eor_abs_y()
{
  start_test_info();

  return 0;
}


static char* test_eor_ind_x()
{
  start_test_info();

  return 0;
}


static char* test_eor_ind_y()
{
  start_test_info();

  return 0;
}

/* ORA tests */

static char* test_ora_im()
{
  start_test_info();

  return 0;
}


static char* test_ora_zp()
{
  start_test_info();

  return 0;
}

static char* test_ora_zp_x()
{
  start_test_info();

  return 0;
}


static char* test_ora_abs()
{
  start_test_info();

  return 0;
}


static char* test_ora_abs_x()
{
  start_test_info();

  return 0;
}


static char* test_ora_abs_y()
{
  start_test_info();

  return 0;
}


static char* test_ora_ind_x()
{
  start_test_info();

  return 0;
}


static char* test_ora_ind_y()
{
  start_test_info();

  return 0;
}

/* BIT test */

static char* test_bit_zp()
{
  start_test_info();

  return 0;
}

static char* test_bit_abs()
{
  start_test_info();

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
