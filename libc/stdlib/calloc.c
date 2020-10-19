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
}

#endif