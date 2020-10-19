#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/heap.h>
#endif

void *realloc(void *ptr, size_t size)
{
#if defined(__is_libk)
    return k_realloc(ptr, size);
#else
    // TODO: Implement syscall
    return NULL;
#endif
}

#ifdef __test

#include <ctest.h>

void test_realloc(void)
{
}

#endif