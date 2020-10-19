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

void test_free(void)
{
}

#endif
