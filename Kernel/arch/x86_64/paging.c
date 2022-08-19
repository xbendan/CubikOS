#include <x86_64/paging.h>
#include <mm/memory.h>
#include <mm/malloc.h>
#include <mm/address.h>
#include <proc/sched.h>
#include <graphic/terminal.h>
#include <macros.h>

pml4_t          kernelPages __attribute__((aligned(ARCH_PAGE_SIZE)));
pdpt_t          kernelPdpts __attribute__((aligned(ARCH_PAGE_SIZE)));
page_dir_t      kernelPageDirs[DIRS_PER_PDPT] __attribute__((aligned(ARCH_PAGE_SIZE)));
page_table_t    kernelPageTables[TABLES_PER_DIR] __attribute__((aligned(ARCH_PAGE_SIZE)));
page_table_t   *kernelPageTablePointers[DIRS_PER_PDPT][TABLES_PER_DIR];
//page_t kernel_pages[TABLES_PER_DIR][PAGES_PER_TABLE] __attribute__((aligned(ARCH_PAGE_SIZE)));

pml4_t *currentPages;

uint64_t kvm_marks[8192];

void LoadVirtualMemory()
{
    pml4_entry_t *pml4Entry = &kernelPages.entries[PDPTS_PER_PML4 - 1];

    pml4Entry->present = 1;
    pml4Entry->writable = 1;
    pml4Entry->usr = 0;

    pml4Entry->addr = (((uint64_t) &kernelPdpts) - KERNEL_VIRTUAL_BASE) / ARCH_PAGE_SIZE;
/*
    *pml4Entry = (pml4_entry_t){
        .usr = 0,
        .writable = 1,
        .present = 1,
        .addr = ((uint64_t) &kernelPdpts) / ARCH_PAGE_SIZE
    };
*/

    for (size_t pdptIndex = 0; pdptIndex < DIRS_PER_PDPT; pdptIndex++)
    {
        pdpt_entry_t *pdptEntry = &kernelPdpts.entries[pdptIndex];

        pdptEntry->present = 1;
        pdptEntry->writable = 1;
        pdptEntry->usr = 0;

        pdptEntry->addr = (((uint64_t) &kernelPageDirs[pdptIndex]) - KERNEL_VIRTUAL_BASE) / ARCH_PAGE_SIZE;
    }

    for (size_t pdirIndex = 0; pdirIndex < TABLES_PER_DIR; pdirIndex++)
    {
        pd_entry_t *pdirEntry = &kernelPageDirs[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)].entries[pdirIndex];

        pdirEntry->present = 1;
        pdirEntry->writable = 1;
        pdirEntry->usr = 0;

        page_table_t *pageTable = &kernelPageTables[pdirIndex];

        pdirEntry->addr = (((uint64_t) pageTable) - KERNEL_VIRTUAL_BASE) / ARCH_PAGE_SIZE;
        kernelPageTablePointers[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)][pdirIndex] = pageTable;
    }
/*
    pdpt_entry_t *pdptEntry = &kernelPdpts.entries[TABLES_PER_DIR - 2];

    pdptEntry->present = 1;
    pdptEntry->writable = 1;
    pdptEntry->usr = 0;

    pdptEntry->addr = ((uint64_t) &kernelPageDirs - KERNEL_VIRTUAL_BASE) / ARCH_PAGE_SIZE;
*/
/*
    *pdptEntry = (pdpt_entry_t){
        .usr = 0,
        .writable = 1,
        .present = 1,
        .addr = ((uint64_t) &kernelPageDirs) / ARCH_PAGE_SIZE
    };
*/
/*
    for (size_t idx = 0; idx < TABLES_PER_DIR; idx++)
    {
        pd_entry_t *kpd_entry = &kernelPageDirs.entries[idx];

        kpd_entry->present = 1;
        kpd_entry->writable = 1;
        kpd_entry->usr = 0;

        kpd_entry->addr = ((uint64_t) &kernelPageTables[idx] - KERNEL_VIRTUAL_BASE) / ARCH_PAGE_SIZE;
    }
*/
    proc_t *kproc = PR_GetKernelProcess();
    //kproc->page_map = &kernelPages;
    kproc->vmmap = &kvm_marks;
    for (size_t idx = 0; idx < 16; idx++)
        kproc->vmmap[idx] = &kvm_marks[idx * 512];

    uint64_t kernel_address = ALIGN_DOWN((uintptr_t) &KERNEL_START_ADDR, ARCH_PAGE_SIZE);
    uint64_t kernel_page_amount = (ALIGN_UP((uintptr_t) &KERNEL_END_ADDR, ARCH_PAGE_SIZE) - kernel_address) / ARCH_PAGE_SIZE;
    
    MapVirtualAddress(
        &kernelPages,
        kernel_address - KERNEL_VIRTUAL_BASE,
        kernel_address,
        kernel_page_amount,
        PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE
    );

    VM_MarkPagesUsed(
        kproc,
        KERNEL_VIRTUAL_BASE,
        (kernel_address - KERNEL_VIRTUAL_BASE) / ARCH_PAGE_SIZE
    );
    VM_MarkPagesUsed(
        kproc,
        kernel_address,
        kernel_page_amount
    );
}

bool IsPagePresent(
    pml4_t *map, 
    uint64_t addr)
{
    size_t pml4Index = PML4_GET_INDEX(addr);

    if(pml4Index == PDPTS_PER_PML4 - 1)
    {
        // Kernel address space
        page_t *page = VM_GetPage(map, addr);
        return page != NULL && page->present;
    }
    else
    {
        size_t pdptIndex = PDPT_GET_INDEX(addr), pdirIndex = PDIR_GET_INDEX(addr), pageIndex = PAGE_GET_INDEX(addr);
    }

/*
    pml4_entry_t *pml4_entry = &map->entries[PML4_GET_INDEX(addr)];
    if (!pml4_entry->present)
    {
        return false;
    }

    pdpt_t *_pdpt = (pdpt_t *)(pml4_entry->addr * ARCH_PAGE_SIZE + kernel_offset);
    pdpt_entry_t *pdpt_entry = &_pdpt->entries[PDPT_GET_INDEX(addr)];
    if (!pdpt_entry->present)
    {
        return false;
    }

    struct page_dir *_page_dir = (struct page_dir *)(pdpt_entry->addr * ARCH_PAGE_SIZE + kernel_offset);
    pd_entry_t *pd_entry = &_page_dir->entries[PDIR_GET_INDEX(addr)];
    if (!pd_entry->present)
    {
        return false;
    }

    struct page_table *_page_table = (struct page_table *)(pd_entry->addr * ARCH_PAGE_SIZE + kernel_offset);
    page_t *page = &_page_table->entries[PAGE_GET_INDEX(addr)];

    return page->present;
*/
}

page_map_t *create_pagemap()
{

}

void destory_pagemap()
{

}

void MapVirtualAddress(
    pml4_t *map,
    uint64_t phys,
    uint64_t virt,
    size_t amount,
    page_flags_t flags)
{
    bool writable = flags & PAGE_FLAG_WRITABLE;
    bool usr = flags & PAGE_FLAG_USER;
    proc_t *kproc = PR_GetKernelProcess();
    size_t pml4Index, pdptIndex, pdirIndex, pageIndex;

    while (amount)
    {
        pml4Index = PML4_GET_INDEX(virt);
        pdptIndex = PDPT_GET_INDEX(virt);
        pdirIndex = PDIR_GET_INDEX(virt);
        pageIndex = PAGE_GET_INDEX(virt);

        if(pml4Index == PDPTS_PER_PML4 - 1)
        {
            /* Kernel Address Space */
            page_t *page = VM_GetPage(map, virt);
            if(page == NULL)
            {
                pd_entry_t *pdirEntry = &kernelPageDirs[pdptIndex].entries[pdirIndex];
                page_table_t *pageTable;
                if(!pdirEntry->present)
                {
                    pdirEntry->present = 1;
                    pdirEntry->writable = flags & PAGE_FLAG_WRITABLE;
                    pdirEntry->usr = flags & PAGE_FLAG_USER;
                    
                    pageTable = (page_table_t *) AllocatePages(kproc, 1);
                    uint64_t pageTablePhys = ConvertVirtToPhys(map, (uint64_t)(pageTable));
                    
                    pdirEntry->addr = pageTablePhys / ARCH_PAGE_SIZE;
                    kernelPageTablePointers[pdptIndex][pdirIndex] = pageTable;
                }
                else
                {
                    pageTable = kernelPageTablePointers[pdptIndex][pdirIndex];
                }
                
                page = &pageTable->entries[pageIndex];
            }

            page->present = 1;
            page->writable = flags & PAGE_FLAG_WRITABLE;
            page->usr = flags & PAGE_FLAG_USER;

            page->addr = phys / ARCH_PAGE_SIZE;
        }
        else
        {
            asm("hlt");
        }

        /*
        pml4_entry_t *pml4Entry = &map->entries[pml4Index];
        if(!pml4Entry->present)
        {
            pml4Entry->present = 1;
            pml4Entry->writable = writable;
            pml4Entry->usr = usr;
            
            pdpt_entry_t *pdptEntry = (pdpt_entry_t*) AllocatePages(kproc, 1);

            pml4Entry->addr = ConvertVirtToPhys(map, (uint64_t)(pdptEntry)) / ARCH_PAGE_SIZE;
        }

        pdpt_t *_pdpt = (pdpt_t *)(pml4_entry->addr * ARCH_PAGE_SIZE + kernel_offset);
        pdpt_entry_t *pdptEntry = &_pdpt->entries[pdptIndex];

        if(!pdptEntry->present)
        {
            pdptEntry->present = 1;
            pdptEntry->writable = writable;
            pdptEntry->usr = usr;
            
            pd_entry_t *pdirEntry = (pd_entry_t*) AllocatePages(kproc, 1);

            pdptEntry->addr = ConvertVirtToPhys(map, (uint64_t)(pdirEntry)) / ARCH_PAGE_SIZE;
        }

        struct page_dir *_pageDir = (struct page_dir *)(pdpt_entry->addr * ARCH_PAGE_SIZE + kernel_offset);
        pd_entry_t *pdirEntry = &_pageDir->entries[pdirIndex];

        if (!pdirEntry->present)
        {
            pdirEntry->present = 1;
            pdirEntry->writable = writable;
            pdirEntry->usr = usr;

            page_t *page = (page_t *) AllocatePages(kproc, 1);

            pdirEntry->addr = ConvertVirtToPhys(map, (uint64_t)(page)) / ARCH_PAGE_SIZE;
        }

        struct page_table *_pageTable = (struct page_table *)(pdirEntry->addr * ARCH_PAGE_SIZE + kernel_offset);
        page_t *page = &_pageTable->entries[pageIndex];

        page->present = 1;
        page->writable = writable;
        page->usr = usr;

        page->addr = phys / ARCH_PAGE_SIZE;
        */

        phys += ARCH_PAGE_SIZE;
        virt += ARCH_PAGE_SIZE;
        amount--;
    }
}

uintptr_t VM_AllocatePages(
    proc_t *process,
    size_t amount)
{
    if(process == NULL)
        process = PR_GetCurrentProcess();
        

    uintptr_t virt = 0;
    bool is_kernel_space = (process == PR_GetKernelProcess());
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
        if (marks[idx] == NULL)
        {
            /* Try to allocate and map 1 page (4096 KiB) for saving marks. */
            uint64_t virt_mark = AllocatePages(
                PR_GetKernelProcess(),
                1);
            if(virt_mark)
                marks[idx] = virt_mark;
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
                if (!(marks[idx][sub_index] & (1 << bit_index)))
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
                        if(is_kernel_space)
                            virt += KERNEL_VIRTUAL_BASE;
                        VM_MarkPagesUsed(process, virt, amount);
                        return virt;
                    }
                }
                else
                    virt = 0;
            }
        }
    }

    return 0;
}

void VM_FreePages(
    proc_t *process,
    uint64_t virt,
    size_t amount)
{
    VM_MarkPagesFree(process, virt, amount);
}

void VM_MarkPagesUsed(
    proc_t *process,
    uint64_t virt, 
    size_t amount)
{

    if(process == PR_GetKernelProcess())
        virt -= KERNEL_VIRTUAL_BASE;

    uint16_t pageIndex, ptr_index;
    uint8_t bit_index;

    uint64_t **marks = process->vmmap;
    
    virt /= ARCH_PAGE_SIZE;
    while (amount)
    {
        ptr_index = virt / 32768;
        pageIndex = virt / 64;
        bit_index = virt % 64;

        process->vmmap[ptr_index][pageIndex] |= (1 << bit_index);
        virt++;
        amount--;

        if(amount <= 0)
        {
            return;
        }
    }
}

void VM_MarkPagesFree(
    proc_t *process,
    uint64_t virt,
    size_t amount)
{
    if(process == PR_GetKernelProcess())
        virt -= KERNEL_ADDR_SPACE;

    uint16_t pageIndex, ptr_index;
    uint8_t bit_index;
        
    virt /= ARCH_PAGE_SIZE;
    while (amount)
    {
        ptr_index = virt / 32768;
        pageIndex = virt / 64;
        bit_index = virt % 64;

        process->vmmap[ptr_index][pageIndex] |= (1 << bit_index);
        virt++;
        amount--;

        if(amount <= 0)
            return;
    }
}

uintptr_t ConvertVirtToPhys(
    pml4_t *map, 
    uintptr_t addr)
{
    uint64_t address = 0;
    size_t pml4Index = PML4_GET_INDEX(addr), pdptIndex = PDPT_GET_INDEX(addr), pdirIndex = PDIR_GET_INDEX(addr), pageIndex = PAGE_GET_INDEX(addr);

    if(pml4Index == PDPTS_PER_PML4 - 1)
    {
        page_table_t *pageTable = kernelPageTablePointers[pdptIndex][pdirIndex];

        if(pageTable != 0x0 && pageTable != NULL)
        {
            return (pageTable->entries[pageIndex].addr * ARCH_PAGE_SIZE) + (addr & 0xFFF);
        }
    }
    else
    {

    }

    return addr;
    
    /*
    pml4_entry_t *pml4_entry = &map->entries[PML4_GET_INDEX(addr)];
    if (!pml4_entry->present)
    {
        return false;
    }

    pdpt_t *_pdpt = (pdpt_t *)(pml4_entry->addr * ARCH_PAGE_SIZE + kernel_offset);
    pdpt_entry_t *pdpt_entry = &_pdpt->entries[PDPT_GET_INDEX(addr)];
    if (!pdpt_entry->present)
    {
        return false;
    }

    struct page_dir *_page_dir = (struct page_dir *)(pdpt_entry->addr * ARCH_PAGE_SIZE + kernel_offset);
    pd_entry_t *pd_entry = &_page_dir->entries[PDIR_GET_INDEX(addr)];
    if (!pd_entry->present)
    {
        return false;
    }

    struct page_table *_page_table = (struct page_table *)(pd_entry->addr * ARCH_PAGE_SIZE + kernel_offset);
    page_t *page = &_page_table->entries[PAGE_GET_INDEX(addr)];

    return (page->addr * ARCH_PAGE_SIZE) + (addr & 0xfff);
    */
}

page_t *VM_GetPage(pml4_t *map, uintptr_t addr)
{
    size_t pml4Index, pdptIndex, pdirIndex, pageIndex;

    pml4Index = PML4_GET_INDEX(addr);
    pdptIndex = PDPT_GET_INDEX(addr);
    pdirIndex = PDIR_GET_INDEX(addr);
    pageIndex = PAGE_GET_INDEX(addr);

    if(pml4Index == (PDPTS_PER_PML4 - 1))
    {
        // Kernel Address Space
        page_table_t *pageTableEntry = kernelPageTablePointers[pdptIndex][pdirIndex]; //kernelPageDirs[pdptIndex].entries[pdirIndex];
        if (pageTableEntry != 0x0)
            return &pageTableEntry->entries[pageIndex];
        else
            return NULL;
    }
    else
    {
        // Process Address Space
        return NULL;
    }
}

void VM_SwitchPageTable(pml4_t *map)
{
    asmi_load_paging(
        ConvertVirtToPhys(
            VM_GetKernelPages(), 
            map
        )
    );
}

pml4_t* VM_CurrentPages() { return asmw_get_pagemap(); }
pml4_t* VM_GetKernelPages() { return &kernelPages; }