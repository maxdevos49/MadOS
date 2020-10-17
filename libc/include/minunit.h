#ifndef __MINUNIT_H
#define __MINUNIT_H

#define mu_assert(message, test) do {  if (!(test)) { test_result = message; return; } } while (0)
#define mu_run_test(test) do { tests_run++; test();  if (test_result) return; } while (0)

extern char *test_result;
extern int tests_run;

void init_test();

#endif