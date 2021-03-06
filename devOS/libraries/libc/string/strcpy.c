#include <string.h>

char *strcpy(char *__restrict dst_ptr, const char *__restrict src_ptr)
{
    return memcpy(dst_ptr, src_ptr, strlen(src_ptr));
}