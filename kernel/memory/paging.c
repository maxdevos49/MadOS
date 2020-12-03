/**
 * Legacy paging
 * */

#include <stdint.h>
#include <stddef.h>
#include <cpuid.h>

extern void PAGING_load_page_directory(uint32_t *directory);
extern void PAGING_enable();

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void PAGING_init()
{
    uint32_t i;
    for (i = 0; i < 1024; i++)
    {
        //Sets the following flags for each directory
        // Supervisor: only kernel can access them.
        // Write Enabled: It can be both read from and written too.
        // Not Present: The page table is not present.
        page_directory[i] = 0x00000002;

        //As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes
        first_page_table[i] = (i * 0x1000) | 3; //attributes: supervisor level, read/write, present
    }

    //attributes: supervisor level, ead/write, present
    page_directory[0] = ((uint32_t)first_page_table) | 3;

    PAGING_load_page_directory(page_directory);
    PAGING_enable();
}

void* PAGING_virtual_to_physical_address()
{
    //TODO
    return NULL;
}

void *PAGING_map()
{
    //TODO
    return NULL;
}

void PAGING_unmap()
{
    //TODO
    return NULL;
}
