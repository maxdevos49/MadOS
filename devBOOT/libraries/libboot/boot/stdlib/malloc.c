#include "../stdlib.h"
#include "../stdint.h"

extern uint64_t _e_bss;
static void *NEXT_FREE_POINTER = NULL;

void *malloc(size_t size)
{
    if (size == 0)
        return NULL;

    if (NEXT_FREE_POINTER == NULL)
        return (NEXT_FREE_POINTER = &_e_bss);

    return NEXT_FREE_POINTER += size;
}