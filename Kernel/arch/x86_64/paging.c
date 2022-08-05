#include <x86_64/paging.h>
#include <mem/memory.h>
#include <mem/malloc.h>
#include <mem/address.h>
#include <proc/sched.h>
#include <macros.h>

pml4_t kernel_page_map __attribute__((aligned(ARCH_PAGE_SIZE)));
pdpt_t kernel_pdpts __attribute__((aligned(ARCH_PAGE_SIZE)));
page_dir_t kernel_heap_dir __attribute__((aligned(ARCH_PAGE_SIZE)));
page_t kernel_pages[TABLES_PER_DIR][PAGES_PER_TABLE] __attribute__((aligned(ARCH_PAGE_SIZE)));

pml4_t *current_pages;

uint64_t kvm_marks[8192];

void vmm_init()
{
    memset(&kernel_page_map, 0, sizeof(pml4_t));
    pml4_entry_t *kpml4_entry = &kernel_page_map.entries[PDPTS_PER_PML4 - 1];

    *kpml4_entry = (pml4_entry_t){
        .usr = 0,
        .writable = 1,
        .present = 1,
        .addr = ((uint64_t) &kernel_pdpts) / ARCH_PAGE_SIZE
    };

    memset(&kernel_pdpts, 0, sizeof(pdpt_t));
    pdpt_entry_t *kpdpt_entry = &kernel_pdpts.entries[0];

    *kpdpt_entry = (pdpt_entry_t){
        .usr = 0,
        .writable = 1,
        .present = 1,
        .addr = ((uint64_t) &kernel_heap_dir) / ARCH_PAGE_SIZE
    };

    memset(&kernel_heap_dir, 0, sizeof(page_dir_t));
    for (size_t idx = 0; idx < TABLES_PER_DIR; idx++)
    {
        pd_entry_t *kpd_entry = &kernel_heap_dir.entries[idx];

        *kpd_entry = (pd_entry_t){
            .usr = 0,
            .writable = 1,
            .present = 1,
            .addr = ((uint64_t) &kernel_pages[idx][0]) / ARCH_PAGE_SIZE
        };
        memset(&kernel_pages[idx][0], 0, sizeof(page_t) * PAGES_PER_TABLE);
    }

    proc_t *kproc = get_kernel_process();
    for (size_t idx = 0; idx < 16; idx++)
        kproc->vmmap[idx] = &kvm_marks[idx * 512];

    uint64_t kernel_address = ALIGN_DOWN((uintptr_t) &KERNEL_START_ADDR, ARCH_PAGE_SIZE);
    uint64_t kernel_page_amount = (ALIGN_UP((uintptr_t) &KERNEL_END_ADDR, ARCH_PAGE_SIZE) - kernel_address) / ARCH_PAGE_SIZE;

    map_virtual_address(
        &kernel_page_map,
        kernel_address,
        kernel_address + KERNEL_VIRTUAL_BASE,
        kernel_page_amount,
        0
    );
    mark_pages_used(
        kproc,
        kernel_address,
        kernel_page_amount
    );

    //mark_pages_used(0x0, 256);
}

bool vmm_page_present(
    pml4_t *map, 
    uint64_t addr)
{
    pml4_entry_t *pml4_entry = (pml4_entry_t*)&((map->entries[pml4_indexof(addr)]));
    if (!pml4_entry->present)
    {
        return false;
    }

    pdpt_entry_t *pdpt_entry = (pdpt_entry_t*)&(((pdpt_t*)(pml4_entry->addr * ARCH_PAGE_SIZE))->entries[pdpt_indexof(addr)]);
    if (!pdpt_entry->present)
    {
        return false;
    }

    pd_entry_t *pd_entry = (pd_entry_t*)&(((page_dir_t*)(pdpt_entry->addr * ARCH_PAGE_SIZE))->entries[pd_indexof(addr)]);
    if (!pd_entry->present)
    {
        return false;
    }

    page_t *page = (page_t*)&(((page_table_t*)(pd_entry->addr * ARCH_PAGE_SIZE))->entries[page_indexof(addr)]);

    return page->present;
}

void map_virtual_address(
    pml4_t *map,
    uint64_t phys,
    uint64_t virt,
    size_t amount,
    page_flags_t flags)
{
    bool writable = flags & PAGE_FLAG_WRITABLE;
    bool usr = flags & PAGE_FLAG_USER;
    proc_t *kproc = get_kernel_process();
    size_t pml4_index, pdpt_index, pd_index, page_index;

    while (amount --)
    {
        pml4_index = pml4_indexof(virt);
        pdpt_index = pdpt_indexof(virt);
        pd_index = pd_indexof(virt);
        page_index = page_indexof(virt);

        pml4_entry_t *pml4_entry = &map->entries[pml4_index];
        
        if(!pml4_entry->present)
        {
            pml4_entry->present = 1;
            pml4_entry->writable = writable;
            pml4_entry->usr = usr;
            
            pdpt_entry_t *pdpt_entry = (pdpt_entry_t*) alloc_pages(kproc, 1);

            pml4_entry->addr = (uint64_t)pdpt_entry / ARCH_PAGE_SIZE;
        }

        pdpt_entry_t *pdpt_entry = &(((pdpt_entry_t*)(pml4_entry->addr * ARCH_PAGE_SIZE))[pdpt_index]);

        if(!pdpt_entry->present)
        {
            pdpt_entry->present = 1;
            pdpt_entry->writable = writable;
            pdpt_entry->usr = usr;

            pd_entry_t *pd_entry = (pd_entry_t*) alloc_pages(kproc, 1);

            pdpt_entry->addr = (uint64_t)pd_entry / ARCH_PAGE_SIZE;
        }

        pd_entry_t *pd_entry = &(((pd_entry_t*)(pdpt_entry->addr * ARCH_PAGE_SIZE))[pd_index]);

        if (!pd_entry->present)
        {
            pd_entry->present = 1;
            pd_entry->writable = writable;
            pd_entry->usr = usr;

            page_t *page = (page_t *) alloc_pages(kproc, 1);

            pd_entry->addr = (uint64_t)page / ARCH_PAGE_SIZE;
        }

        page_t *page = &(((page_t*)(pd_entry->addr * ARCH_PAGE_SIZE))[page_index]);

        page->present = 1;
        page->writable = writable;
        page->usr = usr;

        page->addr = phys / ARCH_PAGE_SIZE;

        phys += ARCH_PAGE_SIZE;
        virt += ARCH_PAGE_SIZE;
    }
}

uintptr_t vmm_alloc_pages(
    proc_t *process,
    size_t amount)
{
    if(process == nullptr)
        process = get_current_process();

    uintptr_t virt = 0;
    bool is_kernel_space = (process == get_kernel_process());
    size_t amount_left;
    size_t space_limit = (is_kernel_space ? 16 : 16 * 256);
    uint64_t** marks = process->vmmap;
    /*
     * Each process can take upto 512 GiB virtual address
     * which is 512GiB / 128MiB = 8 * 512 = 4096
     */
    for (size_t idx = 0; idx < space_limit; idx++)
    {
        /* 
         * Determine whether the pointer point to the page
         * contains page marks is null or not.
         * 
         * If it's a null pointer try to allocate a new page.
         */
        if (marks[idx] == nullptr)
        {
            /* Try to allocate and map 1 page (4096 KiB) for saving marks. */
            uint64_t virt = alloc_pages(
                get_kernel_process(),
                1);
            if(virt)
                marks[idx] = virt;
            else
            {
                /*
                 * If the allocated address is 0 (failed)
                 * Throwing errors about out of address
                 */
                // panic.
            }
        }

        /* Scan 128 MiB virtual address each time */
        for (size_t sub_index = 0; sub_index < 512; sub_index++)
        {
            /* Skip to next mark if all bits set */
            if (marks[idx][sub_index] == 0xffffffffffffffff)
                continue;

            /* Iterate all bits for a free bit */
            for (uint8_t bit_index = 0; bit_index < 64; bit_index++)
            {
                /*
                 * Check whether the bit indicate the page availablitiy
                 * is free or not. 
                 * 
                 * If yes, start or continue counting on the page amount
                 * Otherwise, clear virtual address recorded and prepare
                 * to restart counting.
                 */
                if (!(marks[idx][sub_index] & (1 < sub_index)))
                {
                    /*
                     * If the virtual address is empty, set a new address and
                     * start to count the pages left.
                     */
                    if (!virt)
                    {
                        virt = ((idx * 32768) + (sub_index * 64) + bit_index) * ARCH_PAGE_SIZE;
                        amount_left = amount;
                    }

                    /*
                     * Decrease the amount left and check whether the space taken
                     * is enough for requirement.
                     * 
                     * If yes, mark the virtual memory pages used and return the 
                     * address. (Add kernel virtual base if we are allocating in
                     * kernel address space)
                     * If no, continue to the next loop.
                     */
                    if (!--amount_left)
                    {
                        mark_pages_used(process, virt, amount);
                        return is_kernel_space ? virt : virt + KERNEL_VIRTUAL_BASE;
                    }
                }
                else
                    virt = 0;
            }
        }
    }

    return 0;
}

void vmm_free_pages(
    proc_t *process,
    uint64_t virt,
    size_t amount)
{
    mark_pages_free(process, virt, amount);
}

void mark_pages_used(
    proc_t *process,
    uint64_t virt, 
    size_t amount)
{
    if(process == get_kernel_process())
        virt -= KERNEL_VIRTUAL_BASE;

    uint16_t page_index, ptr_index;
    uint8_t bit_index;
        
    virt /= ARCH_PAGE_SIZE;
    while (amount)
    {
        ptr_index = virt / 32768;
        page_index = virt / 64;
        bit_index = virt % 64;

        process->vmmap[ptr_index][page_index] |= (1 << bit_index);
        virt++;
        amount--;

        if(amount <= 0)
            return;
    }
}

void mark_pages_free(
    proc_t *process,
    uint64_t virt,
    size_t amount)
{
    if(process == get_kernel_process())
        virt -= KERNEL_ADDR_SPACE;

    uint16_t page_index, ptr_index;
    uint8_t bit_index;
        
    virt /= ARCH_PAGE_SIZE;
    while (amount)
    {
        ptr_index = virt / 32768;
        page_index = virt / 64;
        bit_index = virt % 64;

        process->vmmap[ptr_index][page_index] &= ~(1 << bit_index);
        virt++;
        amount--;

        if(amount <= 0)
            return;
    }
}

uintptr_t virt_to_phys(
    pml4_t *map, 
    uintptr_t addr)
{
    pml4_entry_t *pml4_entry = (pml4_entry_t*)&((map->entries[pml4_indexof(addr)]));
    if (!pml4_entry->present)
    {
        return 0;
    }

    pdpt_entry_t *pdpt_entry = (pdpt_entry_t*)&(((pdpt_t*)(pml4_entry->addr * ARCH_PAGE_SIZE))->entries[pdpt_indexof(addr)]);
    if (!pdpt_entry->present)
    {
        return 0;
    }

    pd_entry_t *pd_entry = (pd_entry_t*)&(((page_dir_t*)(pdpt_entry->addr * ARCH_PAGE_SIZE))->entries[pd_indexof(addr)]);
    if (!pd_entry->present)
    {
        return 0;
    }

    page_t *page = (page_t*)&(((page_table_t*)(pd_entry->addr * ARCH_PAGE_SIZE))->entries[page_indexof(addr)]);

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
pml4_t* get_kernel_pages() { return &kernel_pages; }