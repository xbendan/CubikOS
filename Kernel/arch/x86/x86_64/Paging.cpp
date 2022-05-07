#include <Paging.h>
#include <Buddy.h>
#include <GraphicsDevice.h>

using namespace Memory::Allocation;

namespace Paging
{
    const char str[12] = "PAGE_MANAGE";
    pml4_t kpml4 __attribute__((aligned(ARCH_PAGE_SIZE)));
    pdpt_t kpml3 __attribute__((aligned(ARCH_PAGE_SIZE)));
    page_dir_t kHeapDir __attribute__((aligned(ARCH_PAGE_SIZE)));
    //page_table_t kPageTable __attribute__((aligned(ARCH_PAGE_SIZE)));
    /* 
     * Kernel uses 4KiB * 512^2 = 1GiB
     * The page size is 4096 bytes
     * 512 page dir entry in @code kHeapDir @endcode
     * and 512 page table entry in @code kHeapTables @endcode
     * 
     */
    page_t kHeapPages[TABLES_PER_DIR][PAGES_PER_TABLE] __attribute__((aligned(ARCH_PAGE_SIZE)));
    //page_table_t kHeapTables[TABLES_PER_DIR] __attribute__((aligned(ARCH_PAGE_SIZE)));


    void InitializeVirtualMemory()
    {
        memset(kpml4, 0, sizeof(pml4_t));
        memset(kpml3, 0, sizeof(pdpt_t));
        memset(kHeapDir, 0, sizeof(page_dir_t));
        for (size_t idx = 0; idx < TABLES_PER_DIR; idx++)
        {
            memset(&kHeapPages[idx][0], 0, sizeof(page_t) * PAGES_PER_TABLE);
        }

        auto &pml4Entry = kpml4[0];
        pml4Entry.usr = 0;
        pml4Entry.writable = 1;
        pml4Entry.present = 1;
        pml4Entry.addr = (uint64_t)&kpml3 / ARCH_PAGE_SIZE;

        auto &pdptEntry = kpml3[0];
        pdptEntry.usr = 0;
        pdptEntry.writable = 1;
        pdptEntry.present = 1;
        pdptEntry.addr = (uint64_t)&kHeapDir / ARCH_PAGE_SIZE;

        for (size_t var1 = 0; var1 < 512; var1++)
        {
            auto &pdEntry = kHeapDir[var1];
            pdEntry.usr = 0;
            pdEntry.writable = 1;
            pdEntry.present = 1;
            pdEntry.addr = (uint64_t)&kHeapPages[var1][0] / ARCH_PAGE_SIZE;
        }

        uint64_t kernelAddress = ALIGN_DOWN((uintptr_t)&__kmstart__, ARCH_PAGE_SIZE);
        uint64_t pageAmount = (ALIGN_UP((uintptr_t)&__kmend__, ARCH_PAGE_SIZE) - kernelAddress) / ARCH_PAGE_SIZE;
        KernelMapVirtualMemoryAddress(
            kernelAddress,
            kernelAddress,
            pageAmount,
            PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE
        );

        //asm("mov %%rax, %%cr3" ::"a"((uint64_t)kpml4));
        SwitchPml4(&kpml4);
        
        //Interrupts::RegisterInterruptHandler(14, InterruptHandler_PageFault);
    }

    page_map_t* CreateVirtualMemoryMap()
    {
        page_map_t* map;

        map->pml4[0] = map->pml4Entry;
        map->pml4Entry = (pml4_entry_t)
        {
            .present = 1,
            .writable = 1,
            .usr = 1,
            .addr = (uint64_t) &map->pdptEntries[0] / ARCH_PAGE_SIZE
        };
        for (int idx = 0; idx < PDPTS_PER_PML4; idx++)
        {
            map->pdptEntries[idx] = (pdpt_entry_t)
            {
                .present = 0,
                .writable = 1,
                .usr = 1,
                .addr = (uint64_t) &map->pageDirEntries[idx] / ARCH_PAGE_SIZE
            };
        }

        return map;
    }

    void DestoryVirtualMemoryMap(page_map_t* pml4)
    {

    }

    bool IsVirtualPagePresent(page_map_t* map, uintptr_t addr)
    {
        uint64_t pdptIndex = PdptIndexOf(addr);
        uint64_t pdIndex = PdIndexOf(addr);
        uint64_t pageIndex = PtIndexOf(addr);

        if(!map->pdptEntries[pdptIndex].present)
            return false;
        
        if(map->pdptEntries[pdptIndex].addr == 0x0 || map->pageDirEntries[pdptIndex] == nullptr || !map->pageDirEntries[pdptIndex][pdIndex].present)
            return false;

        if(map->pageDirEntries[pdptIndex][pdIndex].addr = 0x0 || map->pages[pdptIndex][pdIndex] == nullptr || !map->pages[pdptIndex][pdIndex][pageIndex].present)
            return false;

        return true;
    }

    void KernelMapVirtualMemoryAddress(uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags)
    {
        uint64_t pdIndex, pageIndex;

        while(amount--)
        {
            pdIndex = PdIndexOf(virt);
            pageIndex = PtIndexOf(virt);

            page_t* pg = &kHeapPages[pdIndex][pageIndex];
            pg->present = 1;
            pg->writable = 1;
            pg->addr = phys;
            //*reinterpret_cast<uint64_t*>(&pg) |= (flags | phys << 12);
            //pg.addr = phys;

            phys += ARCH_PAGE_SIZE;
            virt += ARCH_PAGE_SIZE;
        }
    }

    void KernelMapVirtualMemoryAddress(uint64_t phys, uint64_t virt, size_t amount){
        KernelMapVirtualMemoryAddress(phys, virt, amount, PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE);
    }

    void MapVirtualMemoryAddress(page_map_t *map, uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags)
    {
        size_t pml4Index, pdptIndex, pdIndex, pageIndex;

        while(amount--)
        {
            pml4Index = Pml4IndexOf(virt);
            pdptIndex = PdptIndexOf(virt);
            pdIndex = PdIndexOf(virt);
            pageIndex = PtIndexOf(virt);

            if(pml4Index)
            {
                // Kill process
                // Userspace alert
                return;
            }

            pdpt_entry_t* pdpt = &map->pdptEntries[pdptIndex];

            if(!pdpt->present)
            {
                pdpt->present = 1;
                pdpt->writable = 1;
                pdpt->usr = 1;

                pd_entry_t* pd = (pd_entry_t*)MmBuddyAllocatePage(0)->addr;

                pdpt->addr = (uint64_t) pd / ARCH_PAGE_SIZE;
                map->pageDirEntries[pdptIndex] = pd;
            }

            pd_entry_t* pd = &map->pageDirEntries[pdptIndex][pdIndex];

            if(!pd->present)
            {
                pd->present = 1;
                pd->writable = 1;
                pd->usr = 1;

                page_t* page = (page_t*)MmBuddyAllocatePage(0)->addr;

                pd->addr = (uint64_t) page / ARCH_PAGE_SIZE;
                map->pages[pdptIndex][pdIndex] = page;
            }

            page_t* page = &map->pages[pdptIndex][pdIndex][pageIndex];

            if(!page->present)
            {
                page->present = 1;
                page->writable = 1;
                page->usr = 1;
            }
            map->pages[pdptIndex][pdIndex][pageIndex].addr = phys  / ARCH_PAGE_SIZE;

            /*
            pml4_entry_t* pml4Entry = &(*pml4[pml4Index]);
            pdpt_t* pdpt = reinterpret_cast<pdpt_t*>(pml4Entry->addr * ARCH_PAGE_SIZE);
            if(!pml4Entry->present)
            {
                pdpt = (pdpt_t*)MmBuddyAllocatePage(0)->addr;

                pml4Entry->present = 1;
                pml4Entry->writable = 1;
                pml4Entry->usr = 1;
                pml4Entry->addr = (uintptr_t)pdpt / ARCH_PAGE_SIZE;
            }

            pdpt_entry_t* pdptEntry = &(*pdpt[pdptIndex]);
            page_dir_t* pd = reinterpret_cast<page_dir_t*>(pdptEntry->addr * ARCH_PAGE_SIZE);
            if(!pdptEntry->present)
            {
                pd = (pdpt_t*)MmBuddyAllocatePage(0)->addr;

                pdptEntry->present = 1;
                pdptEntry->writable = 1;
                pdptEntry->usr = 1;
                pdptEntry->addr = (uintptr_t)pd / ARCH_PAGE_SIZE;
            }

            pd_entry_t* pdEntry = &(*pd[pdIndex]);
            page_table_t* pt = reinterpret_cast<page_table_t*>(pdEntry->addr * ARCH_PAGE_SIZE);
            if(!pdEntry->present)
            {
                pt = (pdpt_t*)MmBuddyAllocatePage(0)->addr;

                pdEntry->present = 1;
                pdEntry->writable = 1;
                pdEntry->usr = 1;
                pdEntry->addr = (uintptr_t)pt / ARCH_PAGE_SIZE;
            }

            page_t* pageEntry = &(*pt[pageIndex]);

            pageEntry->present = 1;
            pageEntry->writable = 1;
            pageEntry->usr = MEMORY_FLAG_USER & flags;
            pageEntry->addr = phys / ARCH_PAGE_SIZE;
            */
        }
    }

    void MapVirtualMemoryAddress(page_map_t *map, uint64_t phys, uint64_t virt, size_t amount){
        MapVirtualMemoryAddress(map, phys, virt, amount, PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE | PAGE_FLAG_USER);
    }

    void ValidateVirtualAddress(page_map_t* map, uint64_t addr)
    {
        size_t pdptIndex = PdptIndexOf(addr);
        size_t pdIndex = PdIndexOf(addr);
        size_t pageIndex = PtIndexOf(addr);

        pdpt_entry_t* pdpt = &map->pdptEntries[pdptIndex];

        if(!pdpt->present)
        {
            pdpt->present = 1;
            pdpt->writable = 1;
            pdpt->usr = 1;

            pd_entry_t* pd = (pd_entry_t*)MmBuddyAllocatePage(0)->addr;

            pdpt->addr = (uint64_t) pd / ARCH_PAGE_SIZE;
            map->pageDirEntries[pdptIndex] = pd;
        }

        pd_entry_t* pd = &map->pageDirEntries[pdptIndex][pdIndex];

        if(!pd->present)
        {
            pd->present = 1;
            pd->writable = 1;
            pd->usr = 1;

            page_t* page = (page_t*)MmBuddyAllocatePage(0)->addr;

            pd->addr = (uint64_t) page / ARCH_PAGE_SIZE;
            map->pages[pdptIndex][pdIndex] = page;
        }

        page_t* page = &map->pages[pdptIndex][pdIndex][pageIndex];

        if(!page->present)
        {
            page->present = 1;
            page->writable = 1;
            page->usr = 1;
        }
    }

    void AllocatePages(pml4_t* pml4, uint64_t phys, size_t amount, memory_flags_t flags)
    {
        
    }

    void AllocatePages(pml4_t* pml4, size_t amount, memory_flags_t flags)
    {

    }

    void FreePages(pml4_t* pml4, uint64_t virt, size_t amount)
    {
        
    }

    uintptr_t ConvertVirtToPhys(pml4_t *pml4, uintptr_t vaddr)
    {

    }

    void SwitchPml4(pml4_t* pml4) {
        asmi_load_paging((uint64_t)pml4);
    }
}