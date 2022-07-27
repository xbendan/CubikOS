#include <x86/amd64/paging.h>

void vmm_init();
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

}

void switch_page_tables(pml4_t *map)
{

}

void disable_paging()
{

}

void enable_paging()
{

}

pml4_t* current_page_map();
pml4_t* get_kernel_pages();