#ifndef __MEMORY_H
#define __MEMORY_H

#include <stdint.h>

typedef struct mem_map_entry
{
    uint64_t base_address;
    uint64_t region_length;
    uint32_t region_type;
    uint32_t extended_attributes;
} memory_map_entry_t;

void init_memory(memory_map_entry_t *location);

void print_memory_map(memory_map_entry_t *memory_map);

memory_map_entry_t **get_usable_memory_regions();

#endif