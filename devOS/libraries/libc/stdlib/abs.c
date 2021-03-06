#include <stdlib.h>

int abs(int x)
{
   return __builtin_abs(x);
}

// #ifdef __test

// #include <ctest.h>

// void test_abs(void)
// {
//     ctest_assert("Error: value still negative or changed", abs(-2334) == 2334);
//     ctest_assert("Error: value still negative or changed", abs(34) == 34);
//     ctest_assert("Error: value still negative or changed", abs(-234131234) == 234131234);
//     ctest_assert("Error: value still negative or changed", abs(45632354) == 45632354);
// }

// #endif