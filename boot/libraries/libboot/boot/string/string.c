#include "../string.h"

void *memset(void *buff_ptr, int value, size_t size)
{
    unsigned char *buffer = (unsigned char *)buff_ptr;

    for (size_t i = 0; i < size; i++)
        buffer[i] = (unsigned char)value;

    return buff_ptr;
}

char *strcpy(char *__restrict dst_ptr, const char *__restrict src_ptr)
{
    return memcpy(dst_ptr, src_ptr, strlen(src_ptr));
}

size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;

    return len;
}

void *memcpy(void *__restrict dst_ptr, const void *__restrict src_ptr, size_t size)
{
    unsigned char *dst = (unsigned char *)dst_ptr;
    const unsigned char *src = (const unsigned char *)src_ptr;

    size_t i;
    for (i = 0; i < size; i++)
        dst[i] = src[i];

    return dst_ptr;
}

void *memmove(void *dst_ptr, const void *src_ptr, size_t size)
{
    size_t i;
    uint8_t *dst = (uint8_t *)dst_ptr;
    const uint8_t *src = (uint8_t *)src_ptr;

    if (dst < src)
        for (i = 0; i < size; i++)
            dst[i] = src[i];
    else
        for (i = size; i != 0; i--)
            dst[i - 1] = src[i - 1];

    return dst_ptr;
}