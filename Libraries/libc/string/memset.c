#include <string.h>

void *memset(void *buff_ptr, int value, size_t size)
{
    // if (size <= 8)
    // {
    unsigned char *buffer = (unsigned char *)buff_ptr;

    for (size_t i = 0; i < size; i++)
        buffer[i] = (unsigned char)value;

    return buff_ptr;
    // }

    // // Calculate 64 bit blocks
    // long long int procceding_bytes = size % 8;
    // long long int new_num = size - procceding_bytes;

    // for (long long int *ptr = (long long int *)buff_ptr; ptr < (buff_ptr + new_num); ptr++)
    // {
    //     *ptr=value;
    // }
}

#ifdef __test

#include <ctest.h>

void test_memset(void)
{

    int i;
    int buffer_size = 20;
    char buffer[buffer_size];

    memset(buffer, 0, sizeof(char) * buffer_size);
    for (i = 0; i < buffer_size && buffer[i] == 0; i++)
        ;
    ctest_assert("Error: buffer was not set to all zeros", i == 20);

    memset(buffer, 'g', sizeof(char) * buffer_size);
    for (i = 0; i < buffer_size && buffer[i] == 'g'; i++)
        ;
    ctest_assert("Error: buffer was not set to all 'g'", i == 20);

    memset(buffer, 'G', sizeof(char) * buffer_size);
    for (i = 0; i < buffer_size && buffer[i] == 'G'; i++)
        ;
    ctest_assert("Error: buffer was not set to all 'G", i == 20);
}

#endif
