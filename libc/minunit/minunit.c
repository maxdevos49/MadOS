#include <minunit.h>

#include <stdio.h>
#include <string.h>
#include <kernel/tty.h>

int tests_run;
char *test_result;

void all_tests()
{
    //string
    printf("Testing <string.h>:\n");
    mu_run_test(test_strlen);
    mu_run_test(test_strcmp);

    //stdio
    printf("Testing <stdio.h>:\n");
    mu_run_test(test_printf);

    return;
}

void init_test()
{
    tests_run = 0;
    test_result = (char *)0;

    printf("Running Test:\n");
    all_tests();

    if (test_result != 0)
    {

        terminal_set_theme(VGA_COLOR_BLACK, VGA_COLOR_RED);
        printf("\t\t%s\n", test_result);
        terminal_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
    }
    else
    {
        printf("ALL TEST PASSED\n");
    }

    printf("Tests run: %d\n", tests_run);
}