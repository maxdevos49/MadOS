#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/heap.h>
#endif

void free(void *ptr)
{
#if defined(__is_libk)
    return k_free(ptr);
#else
    // TODO: Implement syscall
    return NULL;
#endif
}

#ifdef __test

#include <ctest.h>
#include <stdint.h>

void test_free(void)
{

    int buffer_size = 30;
    char *buffer = malloc(sizeof(char) * buffer_size);
    free(buffer);
    uint64_t old = (uint64_t)buffer;

    buffer = malloc(sizeof(char) * buffer_size);

    ctest_assert("Error: malloc,free,malloc produces different pointers", old == (uint64_t)buffer);

    //not sure what else to test here at the moment
}

#endif
