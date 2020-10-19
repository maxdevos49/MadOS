#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/heap.h>
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

#ifdef __test

#include <ctest.h>

void test_malloc(void)
{
}

#endif