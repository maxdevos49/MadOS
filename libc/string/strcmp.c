#include <string.h>

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

#ifdef __test

#include <minunit.h>
#include <stdio.h>

void test_strcmp(void)
{
    printf("\tint strcmp(const char *str1, const char *str2)\n");
    mu_assert("Error-1: strings are equal", (strcmp("Hello", "Hello") == 0));
    mu_assert("Error-2: first different value should be greater then zero", (strcmp("Helloo", "Hello") > 0));
    mu_assert("Error-3: first different value should be less then zero", (strcmp("Hello", "Helloo") < 0));
    mu_assert("Error-4: two empty strings are equal", (strcmp("", "") == 0));
    // mu_assert("Error-5: first different value should be greater then zero", (strcmp("What", "") > 0));
    // mu_assert("Error-6: first different value should be less then zero", (strcmp("", "What") < 0));
    mu_assert("Error-7: first different value should be greater then zero", (strcmp("What", "what") > 0));
    mu_assert("Error-8: first different value should be less then zero", (strcmp("what", "What") > 0));

    return;
}

#endif