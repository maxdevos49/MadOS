#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/memory/heap.h>
#endif

void *malloc(size_t size)
{

#if defined(__is_libk)
    return k_malloc(size);
#else
    // TODO: Implement syscall
    return NULL;
#endif
}

// #ifdef __test

// #include <ctest.h>

// void test_malloc(void)
// {

//     int buffer_size = 30;
//     int *buffer = malloc(sizeof(int) * buffer_size);

//     ctest_assert("Error: malloc returned null", buffer != NULL);

//     int *buffer2 = malloc(sizeof(int) * buffer_size);

//     ctest_assert("Error: malloc returned null", buffer2 != NULL);
//     ctest_assert("Error: malloc returned duplicate address without free", buffer != buffer2);
//     ctest_assert("Error: malloc returned overlapping memory", buffer + buffer_size < buffer2);

//     free(buffer);
//     free(buffer2);
// }

// #endif