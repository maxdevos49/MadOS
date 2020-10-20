#ifndef __MEMORY_H
#define __MEMORY_H

#include <stdint.h>

struct mem_map_entry
{
    uint64_t base_address;
    uint64_t region_length;
    uint32_t region_type;
    uint32_t extended_attributes;
};

void init_memory(struct mem_map_entry *location);

void print_all_memory_map(struct mem_map_entry *mem_map);

void print_memory_map(struct mem_map_entry *memory_map);

struct mem_map_entry **get_usable_memory_regions();

#endif