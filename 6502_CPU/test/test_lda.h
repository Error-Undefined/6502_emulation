#include "include_headers.h"

static struct cpu_struct* cpu;
static struct memory_struct* memory;

static void before();

static char* test_lda_im();

static char* all_lda_test();

int run_all_lda_test(struct cpu_struct* cpu_in, struct memory_struct* memory_in);