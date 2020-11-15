#include <string.h>

void *memmove(void *dst_ptr, const void *src_ptr, size_t size)
{
    size_t i;

    if (size < 8)
    {
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
    else
    {
        uint64_t *dst = (uint64_t *)dst_ptr;
        const uint64_t *src = (uint64_t *)src_ptr;

        if (dst < src)
            for (i = 0; i < size / 8; i++)
                dst[i] = src[i];
        else
            for (i = size / 8; i != 0; i--)
                dst[i - 1] = src[i - 1];
    }

    //If the size is not aligned on 8 byte addresses clean up last few bytes
    if (((i * 8) - size) != 0)
    {
        i *= 8;
        uint8_t *dst = (uint8_t *)dst_ptr;
        const uint8_t *src = (uint8_t *)src_ptr;

        if (dst < src)
            for (; i < size; i++)
                dst[i] = src[i];
        else
            for (; i != 0; i--)
                dst[i - 1] = src[i - 1];
    }

    return dst_ptr;
}