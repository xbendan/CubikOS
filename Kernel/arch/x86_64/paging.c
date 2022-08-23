#include <x86_64/paging.h>
#include <x86_64/interrupts.h>
#include <mm/memory.h>
#include <mm/malloc.h>
#include <mm/address.h>
#include <proc/sched.h>
#include <graphic/terminal.h>
#include <macros.h>
#include <panic.h>

pml4_t          kernelPages __attribute__((aligned(ARCH_PAGE_SIZE)));
pdpt_t          kernelPdpts __attribute__((aligned(ARCH_PAGE_SIZE)));
page_dir_t      kernelPageDirs[DIRS_PER_PDPT] __attribute__((aligned(ARCH_PAGE_SIZE)));
page_table_t    kernelPageTables[TABLES_PER_DIR] __attribute__((aligned(ARCH_PAGE_SIZE)));

page_table_t   *kernelPageTablePointers[DIRS_PER_PDPT][TABLES_PER_DIR];

pml4_t *currentPages;

uint64_t kernelPageMarks[8192];

void Interrupts_PageFaultHandler(void *, struct RegisterContext *regs)
{
    WriteLine("[ERR] Page Fault");
    asm("hlt");
}

void LoadVirtualMemory()
{
    pml4_entry_t *pml4Entry = &kernelPages.entries[PDPTS_PER_PML4 - 1];

    pml4Entry->present = 1;
    pml4Entry->writable = 1;
    pml4Entry->usr = 0;

    pml4Entry->addr = (((uint64_t) &kernelPdpts) - KERNEL_VIRTUAL_BASE) / ARCH_PAGE_SIZE;

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

    struct Process *kproc = PR_GetKernelProcess();
    kproc->page_map = &kernelPages;
    kproc->vmmap = (uint64_t **) &kernelPageMarks;
    for (size_t idx = 0; idx < 16; idx++)
        kproc->vmmap[idx] = &kernelPageMarks[idx * 512];

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

    RegisterInterruptHandler(14, &Interrupts_PageFaultHandler);
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
}

page_map_t *VM_CreatePagemap()
{

}

void VM_DestroyPagemap()
{

}

uintptr_t VM_GetIOMapping(uintptr_t address)
{
    if(address > 0xFFFFFFFF)
    {
        WriteLine("[WARN] Access IO address greater than 0xFFFFFFFF.");
        WriteLong(address);
        return 0xFFFFFFFF;
    }
    return address + KERNEL_IO_VIRTUAL_BASE;
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
    struct Process *kproc = PR_GetKernelProcess();
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
                    if(pageTable == NULL || pageTable == 0x0)
                        asm("hlt");
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

        phys += ARCH_PAGE_SIZE;
        virt += ARCH_PAGE_SIZE;
        amount--;
    }
}

uintptr_t VM_AllocatePages(
    struct Process *process,
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
                marks[idx] = (uint64_t *) virt_mark;
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
    struct Process *process,
    uint64_t virt,
    size_t amount)
{
    VM_MarkPagesFree(process, virt, amount);
}

void VM_MarkPagesUsed(
    struct Process *process,
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
    struct Process *process,
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
        if (pageTableEntry != 0x0 && pageTableEntry != NULL)
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
            (uintptr_t) map
        )
    );
}

pml4_t* VM_CurrentPages() { return asmw_get_pagemap(); }
pml4_t* VM_GetKernelPages() { return &kernelPages; }