#ifndef __CTEST_H
#define __CTEST_H

#include <stdbool.h>

void ctest_run_suite(const char *, void (*)(void));
void ctest_assert(const char *, bool);

void run_ctest();

#endif