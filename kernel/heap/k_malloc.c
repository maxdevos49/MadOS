#include <kernel/heap.h>

void *k_malloc(size_t size)
{
    uint64_t rem = size % 8;
    size -= rem;

    if (rem != 0)
        size += 8;

    struct mem_seg_header *current_mem_seg = first_free_mem_seg;

    while (true)
    {
        if (current_mem_seg->length >= size)
        {

            if (current_mem_seg->length > size + sizeof(struct mem_seg_header))
            {
                struct mem_seg_header *new_segment_header =
                    (struct mem_seg_header *)((uint64_t)current_mem_seg + sizeof(struct mem_seg_header) + size);

                new_segment_header->free = true;
                new_segment_header->length = ((uint64_t)current_mem_seg->length) - (sizeof(struct mem_seg_header) + size);
                new_segment_header->next_free_segment = current_mem_seg->next_free_segment;
                new_segment_header->next_segment = current_mem_seg->next_segment;
                new_segment_header->previous_segment = current_mem_seg;
                new_segment_header->previous_free_segment = current_mem_seg->previous_free_segment;

                current_mem_seg->length = size;

                current_mem_seg->next_free_segment = new_segment_header;
                current_mem_seg->next_segment = new_segment_header;
            }

            if (current_mem_seg == first_free_mem_seg)
                first_free_mem_seg = current_mem_seg->next_free_segment;

            current_mem_seg->free = false;

            if (current_mem_seg->previous_free_segment != NULL)
                current_mem_seg->previous_free_segment->next_free_segment = current_mem_seg->next_free_segment;

            if (current_mem_seg->next_free_segment != NULL)
                current_mem_seg->next_free_segment->previous_free_segment = current_mem_seg->previous_free_segment;

            if (current_mem_seg->previous_segment != NULL)
                current_mem_seg->previous_segment->next_free_segment = current_mem_seg->next_free_segment;

            if (current_mem_seg->next_segment != NULL)
                current_mem_seg->next_segment->previous_free_segment = current_mem_seg->previous_free_segment;

            return current_mem_seg + 1;
        }

        if (current_mem_seg->next_free_segment == NULL)
        {
            // abort();
            return NULL; //Should never technically happen unless hit the physical ram limit of the system
        }

        current_mem_seg = current_mem_seg->next_free_segment;
    }

    return 0; // should also never get here;
}