#include <stdio.h>
#include <kernel/memory.h>

static uint8_t usable_memory_regions_count = 0;
static struct mem_map_entry *usable_memory_regions[10];

extern const uint8_t MEMORY_REGION_COUNT; // defined by detect_memory.asm

void print_memory_map(struct mem_map_entry *mem_map)
{
    printf("Memory Base: %x\n", mem_map->base_address);
    printf("Region Length: %d\n", mem_map->region_length);
    printf("Memory Type: %d\n", mem_map->region_type);
    printf("Region Attributes: %x\n\n", mem_map->extended_attributes);
}

void init_memory(struct mem_map_entry *mem_map)
{
    printf("Initializing Memory Map\n");
    printf("Memory Region Count: %d\n", MEMORY_REGION_COUNT);

    //get the usable memory regions
    for (uint8_t i = 0; i < MEMORY_REGION_COUNT; i++)
    {
        if (mem_map[i].region_type == 1)
        {
            usable_memory_regions[usable_memory_regions_count] = &mem_map[i];
            usable_memory_regions_count++;
        }
    }

    printf("Usable Memory Region Count: %d\n", usable_memory_regions_count);
}

struct mem_map_entry **get_usable_memory_regions()
{
    return usable_memory_regions;
}