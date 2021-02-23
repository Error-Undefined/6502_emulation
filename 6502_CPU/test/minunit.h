#include<stdio.h>

#ifndef MINUNIT_H
#define MINUNIT_H

// Self added macro for visibility
#define start_test_info() do{printf("Starting test %s, test nr %d\n",__func__, tests_run + 1);} while (0)

/* file: minunit.h */
 #define mu_assert(message, test) do {if (!(test)) return message; } while (0)
 #define mu_run_test(test) do {char *message = test(); tests_run++; \
                                if (message) return message; } while (0)
 extern int tests_run;

 #endif