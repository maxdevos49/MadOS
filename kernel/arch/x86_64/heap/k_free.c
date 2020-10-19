#include <kernel/heap.h>

static void combine_free_segments(struct mem_seg_header *a, struct mem_seg_header *b);

void k_free(void *address)
{
    struct mem_seg_header *current_memory_segment = ((struct mem_seg_header *)address) - 1;
    current_memory_segment->free = true;

    if (current_memory_segment < first_free_mem_seg)
        first_free_mem_seg = current_memory_segment;

    if (current_memory_segment->next_free_segment != NULL)
    {
        if (current_memory_segment->next_free_segment->previous_free_segment < current_memory_segment)
            current_memory_segment->next_free_segment->previous_free_segment = current_memory_segment;
    }

    if (current_memory_segment->previous_free_segment != 0)
    {
        if (current_memory_segment->previous_free_segment->next_free_segment > current_memory_segment)
            current_memory_segment->previous_free_segment->next_free_segment = current_memory_segment;
    }

    if (current_memory_segment->next_segment != NULL)
    {
        current_memory_segment->next_segment->previous_segment = current_memory_segment;

        if (current_memory_segment->next_segment->free)
            combine_free_segments(current_memory_segment, current_memory_segment->next_segment);
    }

    if (current_memory_segment->previous_segment != NULL)
    {
        current_memory_segment->previous_segment->next_segment = current_memory_segment;
        if (current_memory_segment->previous_segment->free)
            combine_free_segments(current_memory_segment, current_memory_segment->previous_segment);
    }
}

static void combine_free_segments(struct mem_seg_header *a, struct mem_seg_header *b)
{
    if (a == NULL || b == NULL || a == b)
        return;

    if (a < b)
    {
        a->length += b->length + sizeof(struct mem_seg_header);
        a->next_segment = b->next_segment;
        a->next_free_segment = b->next_free_segment;
        b->next_segment->previous_segment = a;
        b->next_segment->previous_free_segment = a;
        b->next_free_segment->previous_free_segment = a;
    }
    else
    {
        b->length += a->length + sizeof(struct mem_seg_header);
        b->next_segment = a->next_segment;
        b->next_free_segment = a->next_free_segment;
        a->next_segment->previous_segment = b;
        a->next_segment->previous_free_segment = b;
        a->next_free_segment->previous_free_segment = b;
    }
}
