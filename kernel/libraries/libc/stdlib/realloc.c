#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/memory/heap.h>
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

// #ifdef __test

// #include <ctest.h>
// #include <string.h>

// void test_realloc(void)
// {
//     int buffer_size = 30;
//     char *buffer = malloc(sizeof(char) * buffer_size);

//     memset(buffer, 'A', sizeof(char) * buffer_size);

//     char *new_buffer = realloc(buffer, sizeof(char) * buffer_size);

//     ctest_assert("Error: same pointer returned", buffer != new_buffer);

//     int i;
//     for (i = 0; i < buffer_size && new_buffer[i] == 'A'; i++)
//         ;

//     ctest_assert("Error: reallocated pointer did not maintain data", i == buffer_size);

//     free(new_buffer);
// }

// #endif