#include <stdlib.h>

static unsigned long int next = 1;

int rand(void)
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % RAND_MAX;
}

void srand(unsigned int seed)
{
    next = seed;
}

// #if __test

// #include <ctest.h>

// void test_rand()
// {
//     srand(0);

//     int i, r;
//     for (i = 0; i < 1000 && r > -1 && r < RAND_MAX; i++, r = rand())
//         ;
//     ctest_assert("Error: rand outputs invalid number.", i == 1000);
// }

// #endif