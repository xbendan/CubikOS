#include <x86/amd64/paging.h>
#include <mem/memory.h>

pml4_t k_pages __attribute__((aligned(ARCH_PAGE_SIZE)));
pdpt_t kpml3 __attribute__((aligned(ARCH_PAGE_SIZE)));
page_dir_t k_heapdir __attribute__((aligned(ARCH_PAGE_SIZE)));

void vmm_init()
{

}

bool vmm_page_present(
    pml4_t *map, 
    uint64_t addr
)
{
    
}

void map_virtual_address(
    uint64_t phys,
    uint64_t virt,
    size_t amount,
    page_flags_t flags
)
{

}

uintptr_t vmm_alloc_pages(size_t amount)
{

}

void vmm_free_pages(
    uint64_t virt,
    size_t amount
)
{

}

void mark_pages_used(
    uint64_t virt, 
    size_t amount
)
{

}

void mark_pages_free(
    uint64_t virt,
    size_t amount
)
{

}

uintptr_t virt_to_phys(
    pml4_t *pml4, 
    uintptr_t addr
)
{
    vm_pml4_entry_t *pml4_entry = (vm_pml4_entry_t*)&((pml4->entries[pml4_indexof(addr)]));
    if (!pml4_entry->present)
    {
        return 0;
    }

    vm_pdpt_entry_t *pdpt_entry = (vm_pdpt_entry_t*)&(((pdpt_t*)(pml4_entry->addr * ARCH_PAGE_SIZE))->entries[pdpt_indexof(addr)]);
    if (!pdpt_entry->present)
    {
        return 0;
    }

    vm_pd_entry_t *pd_entry = (vm_pd_entry_t*)&(((page_dir_t*)(pdpt_entry->addr * ARCH_PAGE_SIZE))->entries[pd_indexof(addr)]);
    if (!pd_entry->present)
    {
        return 0;
    }

    vm_page_t *page = (vm_page_t*)&(((page_table_t*)(pd_entry->addr * ARCH_PAGE_SIZE))->entries[page_indexof(addr)]);

    if (!page->present && !page->addr)
    {
        return 0;
    }

    return (page->addr * ARCH_PAGE_SIZE) + (addr & 0xfff);
}

void switch_page_tables(pml4_t *map)
{
    asmi_load_paging((uint64_t) map);
}

pml4_t* current_page_map() { return asmw_get_pagemap(); }
pml4_t* get_kernel_pages() { return &k_pages; }