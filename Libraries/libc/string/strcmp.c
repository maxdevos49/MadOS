#include <string.h>

int strcmp(const char *s1, const char *s2)
{
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    while (*p1 && *p1 == *p2)
        ++p1, ++p2;

    return (*p1 > *p2) - (*p2 > *p1);
}

// #ifdef __test

// #include <ctest.h>

// void test_strcmp(void)
// {
//     ctest_assert("Error-1: strings are equal", strcmp("Hello", "Hello") == 0);
//     ctest_assert("Error-2: first different value should be greater then zero", strcmp("Helloo", "Hello") != 0);
//     ctest_assert("Error-3: first different value should be less then zero", strcmp("Hello", "Helloo") != 0);
//     ctest_assert("Error-4: two empty strings are equal", strcmp("", "") == 0);
//     ctest_assert(("Error-5: first different value should be greater then zero"), (strcmp("Test", "") > 0));
//     ctest_assert("Error-6: first different value should be less then zero", (strcmp("", "What") < 0));
//     ctest_assert("Error-7: first different value should be greater then zero", (strcmp("What", "what") < 0));
//     ctest_assert("Error-8: first different value should be less then zero", (strcmp("what", "What") > 0));

//     return;
// }

// #endif