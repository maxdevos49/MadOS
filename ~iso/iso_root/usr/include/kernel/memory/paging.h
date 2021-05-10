#ifndef __KERNEL_MEMORY_PAGING_H
#define __KERNEL_MEMORY_PAGING_H 1

#include <kernel/interrupts/idt.h>

#define PML4_offset_mask (uint64_t)0x0000ff8000000000
#define PDPE_offset_mask (uint64_t)0x0000007fc0000000
#define PDE_offset_mask (uint64_t)0x000000003fe00000
#define PTE_offset_mask (uint64_t)0x00000000001ff000
#define PP_offset_mask (uint64_t)0x0000000000000fff

#define PE_address_mask (uint64_t)0x000ffffffffff000
#define PE_present_mask (uint64_t)0x0000000000000001
#define PE_rw_mask (uint64_t)0x0000000000000002
#define PE_userspace_mask (uint64_t)0x0000000000000004
#define PE_pwt_mask (uint64_t)0x0000000000000008

#define PML4_offset(address) (((uint64_t)address & PML4_offset_mask) >> 39)
#define PDPE_offset(address) (((uint64_t)address & PDPE_offset_mask) >> 30)
#define PDE_offset(address) (((uint64_t)address & PDE_offset_mask) >> 21)
#define PTE_offset(address) (((uint64_t)address & PTE_offset_mask) >> 12)
#define PP_offset(address) ((uint64_t)address & PP_offset_mask)

//Page Bitmask
#define PAGE_PRES 0x0000000000000001
#define PAGE_RW 0x0000000000000002
#define PAGE_USER 0x0000000000000004
//TODO ...

void PAGING_init();
void PAGING_fault(struct registers *regs);

uint64_t *PAGING_virt_to_phys_address(void *virtual_address);
void PAGING_switch_cr3(uint64_t cr3);

void PAGING_map(void *virtual_address, uint64_t flags);
void PAGING_unmap(void *virtual_address);

static inline uint64_t PAGING_get_cr3()
{
    uint64_t cr3;
    asm volatile(
        "mov %%cr3, %%rax\n\t"
        "mov %%rax, %0\n\t"
        : "=m"(cr3)
        :
        : "%rax");
    return cr3;
}

static inline uint64_t PAGING_get_pml4_entry(uint64_t cr3, uint64_t *address)
{
    return ((uint64_t *)cr3)[PML4_offset(address)];
}

static inline uint64_t PAGING_get_pdp_entry(uint64_t pml4_entry, uint64_t *address)
{
    return ((uint64_t *)(pml4_entry & PE_address_mask))[PDPE_offset(address)];
}

static inline uint64_t PAGING_get_pd_entry(uint64_t pdp_entry, uint64_t *address)
{
    return ((uint64_t *)(pdp_entry & PE_address_mask))[PDE_offset(address)];
}

static inline uint64_t PAGING_get_pt_entry(uint64_t pd_entry, uint64_t *address)
{
    return ((uint64_t *)(pd_entry & PE_address_mask))[PTE_offset(address)];
}

#endif