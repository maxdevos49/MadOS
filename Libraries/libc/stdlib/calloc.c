#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/heap.h>
#endif

void *calloc(size_t num, size_t size)
{
#if defined(__is_libk)
    return k_calloc(num, size);
#else
    // TODO: Implement syscall
    return NULL;
#endif
}

#ifdef __test

#include <ctest.h>

void test_calloc(void)
{
    int buffer_size = 30;
    int *buffer = calloc(buffer_size, sizeof(int));
    ctest_assert("Error: buffer1 calloc returned null", buffer != NULL);

    int *buffer2 = calloc(buffer_size, sizeof(int));
    ctest_assert("Error: buffer2 calloc returned null", buffer2 != NULL);

    ctest_assert("Error: calloc returned duplicate address without free", buffer != buffer2);
    ctest_assert("Error: calloc returned overlapping memory", buffer + buffer_size < buffer2);//wrong

    int i;
    for (i = 0; i < buffer_size && buffer[i] == 0; i++)
        ;
    ctest_assert("Error: calloc did not zero out memory", i == buffer_size);

    for (i = 0; i < buffer_size && buffer2[i] == 0; i++)
        ;
    ctest_assert("Error: calloc did not zero out memory", i == buffer_size);

    free(buffer);
    free(buffer2);
}

#endif