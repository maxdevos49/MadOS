#include <string.h>

size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;

    return len;
}

#ifdef __test

#include <minunit.h>
#include <stdio.h>

void test_strlen(void)
{
    printf("\tsize_t strlen(const char *str)\n");
    mu_assert("Error length should be 4 for string \"test\"", strlen("Test") == 4);
    mu_assert("Error length should be 0 for string \"\"", strlen("") == 0);
    mu_assert("Error length should be 6 for string \"testin\\0g\"", strlen("testin\0g") == 6);
}

#endif