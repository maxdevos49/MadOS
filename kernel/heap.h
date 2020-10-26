#ifndef __KERNEL_HEAP_H
#define __KERNEL_HEAP_H 1

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct mem_seg_header;

struct mem_seg_header
{
    uint64_t length;
    struct mem_seg_header *next_segment;
    struct mem_seg_header *previous_segment;
    struct mem_seg_header *next_free_segment;
    struct mem_seg_header *previous_free_segment;
    bool free;
};

extern struct mem_seg_header *first_free_mem_seg;

void init_heap(uint64_t heap_address, uint64_t heap_length);

void *k_malloc(size_t size);
void *k_realloc(void *ptr, size_t size);
void *k_calloc(size_t num, size_t size);
void k_free(void *ptr);

#endif