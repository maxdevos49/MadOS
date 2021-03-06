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
    //     *ptr = value;
    // }
}