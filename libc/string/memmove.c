#include <string.h>

void *memmove(void *dst_ptr, const void *src_ptr, size_t size)
{
    
    //Must cast void pointers
    unsigned char *dst = (unsigned char *)dst_ptr;
    const unsigned char *src = (const unsigned char *)src_ptr;

    //depending which pointer has a higher memory address we do not want to overwrite any data because its possible the two regions can overlap
    if (dst > src)
    {
        for (size_t i = 0; i < size; i++)
        {
            dst[i] = src[i];
        }
    }
    else
    {
        for (size_t i = size; i != 0; i--)
        {
            dst[i-1] = src[i-1];
        }
    }

    return dst_ptr;
}