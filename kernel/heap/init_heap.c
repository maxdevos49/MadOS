#include <kernel/heap.h>
#include <stdio.h>
#include <stdlib.h>

struct mem_seg_header *first_free_mem_seg;

void init_heap(uint64_t base_address, uint64_t length)
{
    printf("Initializing heap\n");

    printf("\tHeap Base Address... 0x%x\n", base_address);
    printf("\tHeap size... %d\n", length);

    first_free_mem_seg = (struct mem_seg_header *)base_address;
    first_free_mem_seg->length = length - sizeof(struct mem_seg_header);
    first_free_mem_seg->next_segment = NULL;
    first_free_mem_seg->previous_segment = NULL;
    first_free_mem_seg->next_free_segment = NULL;
    first_free_mem_seg->free = true;
}




