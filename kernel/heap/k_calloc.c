#include <kernel/heap/heap.h>
#include <string.h>

void *k_calloc(size_t num, size_t size)
{
    void* address = k_malloc(size * num);

    memset(address, 0,size * num);

    return address;
}