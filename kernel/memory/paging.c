#include <kernel/memory/paging.h>

#include <string.h>

const uint64_t *KERNEL_PML4 = 0x1000;

/**
 * Reinitialize paging while taking advantage of long mode
 * */
void PAGING_init()
{
    memset(KERNEL_PML4, 0, sizeof(uint64_t) * 512);

    for (int i = 0; i < 194; i++) //identity maps up to 0xbffff
        PAGING_map(i * 0x1000, PAGE_PRES | PAGE_RW);

    //TODO virtual map heap??

    PAGING_switch_cr3(KERNEL_PML4);
}

static uint64_t *PAGING_first_available_physical_frame()
{
    //TODO;
    return NULL;
}

void PAGING_map(void *virtual_address, uint64_t flags)
{
    uint64_t *physical_address = PAGING_first_available_physical_frame();

    printf("Mapping Page: %x -> %x\n", physical_address, virtual_address);
    printf("PML4: %d, PDPE: %d, PDE: %d, PTE: %d, PP: %x", PML4_offset(virtual_address), PDPE_offset(virtual_address), PDE_offset(virtual_address), PTE_offset(virtual_address), PP_offset(virtual_address));

    uint64_t pml4_entry = PAGING_get_pml4_entry(PAGING_get_cr3(), virtual_address);
    if (!(pml4_entry & PE_present_mask))
    {
        uint64_t *table = kmalloc_a(sizeof(uint64_t) * 512); //TODO
        memset64(table, 0, sizeof(uint64_t) * 512);
        pml4_entry = ((uint64_t)table & PE_address_mask) | PE_present_mask | PE_rw_mask;
    }

    uint64_t pdp_entry = PAGING_get_pdp_entry(pml4_entry, virtual_address);
    if (!(pdp_entry & PE_present_mask))
    {
        uint64_t *table = kmalloc_a(sizeof(uint64_t) * 512); //TODO
        memset64(table, 0, sizeof(uint64_t) * 512);
        pdp_entry = ((uint64_t)table & PE_address_mask) | PE_present_mask | PE_rw_mask;
    }

    uint64_t pd_entry = PAGING_get_pd_entry(pdp_entry, virtual_address);
    if (!(pd_entry & PE_present_mask))
    {
        uint64_t *table = kmalloc_a(sizeof(uint64_t) * 512); //TODO
        memset64(table, 0, sizeof(uint64_t) * 512);
        pd_entry = ((uint64_t)table & PE_address_mask) | PE_present_mask | PE_rw_mask;
    }

    uint64_t pt_entry = PAGING_get_pdp_entry(pml4_entry, virtual_address);
    if (pdp_entry & PE_present_mask)
        return; //already mapped

    //todo
}

void PAGING_unmap(void *virtual_address)
{
    //TODO
}

uint64_t *PAGING_virt_to_phys_address(void *virtual_address)
{
    uint64_t pml4_entry = PAGING_get_pml4_entry(PAGING_get_cr3(), virtual_address);
    if (!(pml4_entry & PE_present_mask))
        return NULL;

    uint64_t pdp_entry = PAGING_get_pdp_entry(pml4_entry, virtual_address);
    if (!(pdp_entry & PE_present_mask))
        return NULL;

    uint64_t pd_entry = PAGING_get_pd_entry(pdp_entry, virtual_address);
    if (!(pd_entry & PE_present_mask))
        return NULL;

    uint64_t pt_entry = PAGING_get_pt_entry(pd_entry, virtual_address);
    if (!(pt_entry & PE_present_mask))
        return NULL;

    return (pt_entry & PE_address_mask) | PP_offset(virtual_address);
}

void PAGING_fault(struct registers *regs)
{
    //TODO when support is added for this
}
