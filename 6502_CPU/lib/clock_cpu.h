#include <pthread.h>

#include "data_types.h"

#ifndef CLOCK_H
#define CLOCK_H

typedef struct clock_emu_struct clock_emu_type;

struct clock_emu_struct
{
  // The clock frequency in 
  u32 frequency;
  pthread_t* clock_thread; 
};


void clock_init(clock_emu_type* clock_type);

void clock_step(clock_emu_type* clock_type);

#endif
