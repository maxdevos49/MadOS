#include <kernel/memory/heap.h>
#include <string.h>

void *k_realloc(void *ptr, size_t size)
{
    void *new_ptr = k_malloc(size);
    memcpy(new_ptr, ptr, size);
    k_free(ptr);

    return new_ptr;
}