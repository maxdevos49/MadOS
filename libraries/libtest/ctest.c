#include <ctest.h>
#include <stdio.h>


struct ctest_data
{
    int tests_run;
    int invalid_test;
    int valid_test;
};

static struct ctest_data test_data;

void ctest_assert(const char *error_msg, bool is_true)
{
    test_data.tests_run++;

    if (!is_true)
    {
        test_data.invalid_test++;
        TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_RED);
        printf("\t\t%s\n", error_msg);
        TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
    }
    else
    {
        test_data.valid_test++;
    }
}

void ctest_run_suite(const char *suite_name, void (*test_suite)(void))
{
    printf("\t%s\n", suite_name);
    test_suite();
}

void run_ctest(void (*ctest)(void))
{

    if (ctest == NULL)
    {
        TTY_set_theme(VGA_COLOR_RED, VGA_COLOR_WHITE);
        printf("Invalid ctest function\n");
        TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
        return;
    }

    test_data.tests_run = 0;
    test_data.invalid_test = 0;
    test_data.valid_test = 0;

    TTY_set_theme(VGA_COLOR_GREEN, VGA_COLOR_WHITE);
    printf("\t Running Unit Test \n");
    TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
    printf("=================================================\n\n");

    ctest();

    printf("\n=================================================\n");
    if (test_data.invalid_test == 0)
    {
        TTY_set_theme(VGA_COLOR_GREEN, VGA_COLOR_WHITE);
        printf("\t ALL TEST PASSED \n");
        TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
    }
    else
    {
        TTY_set_theme(VGA_COLOR_RED, VGA_COLOR_WHITE);
        printf("\tTEST FAILED\n");
        TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
    }
    printf("\n\tTest run: %d\n\tTest Failed: %d\n\tTest Passed: %d\n", test_data.tests_run, test_data.invalid_test, test_data.valid_test);
}