#include <string.h>

int memcmp(const void *ptr1, const void *ptr2, size_t size)
{
    //cast so we can use them
    char *buffer1 = (char *)ptr1;
    char *buffer2 = (char *)ptr2;

    size_t i;
    for (i = 0; buffer1[i] - buffer2[i] == 0 && i < size; i++)
        ;

    return buffer1[i] - buffer2[i];
}