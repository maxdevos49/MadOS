#include <string.h>
#include <stdint.h>
#include <stdio.h>

void *memset64(void *buff_ptr, uint64_t value, size_t size)
{
    printf("Warning memset64 is broken\n");
    size_t i = 0;

    if (size < 8)
    {
        while (size != i)
            ((char *)buff_ptr)[i++] = (char)value;

        return buff_ptr;
    }

    while (i < (size / 8))
    {
        ((uint64_t *)buff_ptr)[i] = value;
        i++;
    }

    i *= 8;
    while (i < size)
        ((uint8_t *)buff_ptr)[i++] = (uint8_t)value;

    return buff_ptr;
}