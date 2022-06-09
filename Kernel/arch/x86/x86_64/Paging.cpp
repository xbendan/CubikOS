#include <Paging.h>
#include <mem/Buddy.h>
#include <mem/MemoryAllocation.h>
#include <graphic/GraphicsDevice.h>


namespace Paging
{
    const char str[12] = "PAGE_MANAGE";
    pml4_t kpml4 __attribute__((aligned(ARCH_PAGE_SIZE)));
    pdpt_t kpml3 __attribute__((aligned(ARCH_PAGE_SIZE)));
    page_dir_t kHeapDir __attribute__((aligned(ARCH_PAGE_SIZE)));
    page_map_t* currentPages;
    //page_table_t kPageTable __attribute__((aligned(ARCH_PAGE_SIZE)));
    /* 
     * Kernel uses 4KiB * 512^2 = 1GiB
     * The page size is 4096 bytes
     * 512 page dir entry in @code kHeapDir @endcode
     * and 512 page table entry in @code kHeapTables @endcode
     * 
     */
    page_t kHeapPages[TABLES_PER_DIR][PAGES_PER_TABLE] __attribute__((aligned(ARCH_PAGE_SIZE)));
    uint64_t pageMarkers[4096];
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

        pml4_entry_t* pml4Entry = &kpml4[0];
        pml4Entry->usr = 0;
        pml4Entry->writable = 1;
        pml4Entry->present = 1;
        pml4Entry->addr = (uint64_t)&kpml3 / ARCH_PAGE_SIZE;

        pdpt_entry_t* pdptEntry = &kpml3[0];
        pdptEntry->usr = 0;
        pdptEntry->writable = 1;
        pdptEntry->present = 1;
        pdptEntry->addr = (uint64_t)&kHeapDir / ARCH_PAGE_SIZE;

        for (size_t var1 = 0; var1 < 512; var1++)
        {
            pd_entry_t* pdEntry = &kHeapDir[var1];
            pdEntry->usr = 0;
            pdEntry->writable = 1;
            pdEntry->present = 1;
            pdEntry->addr = (uint64_t)&kHeapPages[var1][0] / ARCH_PAGE_SIZE;
        }

        KernelMarkPagesIdentity(0, 256);

        uint64_t krnlAddress = ALIGN_DOWN((uintptr_t)&__kmstart__, ARCH_PAGE_SIZE);
        uint64_t krnlPageAmount = (ALIGN_UP((uintptr_t)&__kmend__, ARCH_PAGE_SIZE) - krnlAddress) / ARCH_PAGE_SIZE;
        KernelMapVirtualAddress(
            krnlAddress,
            krnlAddress,
            krnlPageAmount
        );
        KernelMarkPagesIdentity(
            krnlAddress,
            krnlPageAmount
        );
        
        //Interrupts::RegisterInterruptHandler(14, InterruptHandler_PageFault);
    }

    page_map_t* CreateVirtualMemoryMap()
    {
        page_map_t* map;
        // Incompleted

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

    void KernelMapVirtualAddress(uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags)
    {
        uint64_t pdIndex, pageIndex;

        while(amount--)
        {
            pdIndex = PdIndexOf(virt);
            pageIndex = PtIndexOf(virt);

            page_t* pg = &kHeapPages[pdIndex][pageIndex];
            pg->present = 1;
            pg->writable = 1;
            pg->addr = phys / ARCH_PAGE_SIZE;
            //*reinterpret_cast<uint64_t*>(&pg) |= (flags | phys << 12);
            //pg.addr = phys;

            phys += ARCH_PAGE_SIZE;
            virt += ARCH_PAGE_SIZE;
        }
    }

    void KernelMapVirtualAddress(uint64_t phys, uint64_t virt, size_t amount){
        KernelMapVirtualAddress(phys, virt, amount, PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE);
    }

    uintptr_t KernelAllocatePages(size_t amount)
    {
        uintptr_t vaddr = 0;
        size_t amountLeft;

        for(uint16_t idx = 0; idx < 4096; idx++)
        {
            if(pageMarkers[idx] == 0xFFFFFFFFFFFFFFFF)
            {
                vaddr = 0;
                continue;
            }

            for(uint8_t bitIndex = 0; bitIndex < 64; bitIndex++)
            {
                if(pageMarkers[idx] & (1 << bitIndex))
                    vaddr = 0;
                else
                {
                    if(!vaddr)
                    {
                        vaddr = (idx * 64 + bitIndex) * ARCH_PAGE_SIZE;
                        amountLeft = amount;
                    }
                    
                    amountLeft--;
                    if(!amountLeft)
                    {
                        KernelMarkPagesIdentity(vaddr, amount);
                        return vaddr;
                    }
                }
            }
        }

        /* Panic! Out of address spaces */
        return 0;
    }

    void KernelFreePages(uint64_t virt, size_t amount)
    {
        uint16_t pageIndex;
        uint8_t bitIndex;
        
        virt /= ARCH_PAGE_SIZE;
        while (amount)
        {
            pageIndex = virt / 64;
            bitIndex = virt % 64;

            pageMarkers[pageIndex] &= (~(1 << bitIndex));
            virt++;
            amount--;

            if(amount <= 0)
                return;
        }
    }

    void KernelMarkPagesIdentity(uint64_t virt, size_t amount)
    {
        uint16_t pageIndex;
        uint8_t bitIndex;
        
        virt /= ARCH_PAGE_SIZE;
        while (amount)
        {
            pageIndex = virt / 64;
            bitIndex = virt % 64;

            pageMarkers[pageIndex] |= (1 << bitIndex);
            virt++;
            amount--;

            if(amount <= 0)
                return;
        }

        /*
        for(uint16_t idx = virt / 64; idx < 4096; idx++)
        {
            for(uint8_t bitIndex = 0; bitIndex < 64; bitIndex++)
            {
                if(idx * 64 + bitIndex != virt)
                    continue;
                
                pageMarkers[idx] |= (1 << (63 - bitIndex));
                virt++;
                amount--;

                if(amount <= 0)
                    return;
            }
        }
        */
    }

    void MapVirtualAddress(page_map_t *map, uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags)
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

                pd_entry_t* pd = (pd_entry_t*)MmBuddyAllocatePages(0)->addr;

                pdpt->addr = (uint64_t) pd / ARCH_PAGE_SIZE;
                map->pageDirEntries[pdptIndex] = pd;
            }

            pd_entry_t* pd = &map->pageDirEntries[pdptIndex][pdIndex];

            if(!pd->present)
            {
                pd->present = 1;
                pd->writable = 1;
                pd->usr = 1;

                page_t* page = (page_t*)MmBuddyAllocatePages(0)->addr;

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
        }
    }

    void MapVirtualAddress(page_map_t *map, uint64_t phys, uint64_t virt, size_t amount){
        MapVirtualAddress(map, phys, virt, amount, PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE | PAGE_FLAG_USER);
    }

    uintptr_t AllocatePages(page_map_t* map, size_t amount)
    {
        uintptr_t vaddr = 0;
        size_t amountLeft;
        /* 
         * Each process can take upto 512 GiB virtual address
         * which is 512GiB / 128MiB = 8 * 512 = 4096
         */
        for(size_t idx = 0; idx < 8 * 512; idx++)
        {
            if(map->pageMarks[idx] == nullptr)
            {
                uint64_t virt = KernelAllocatePages(1);
            }

            /* Scan 128 MiB virtual address each time */
            for(size_t subIndex = 0; subIndex < 512; subIndex++)
            {
                if(map->pageMarks[idx][subIndex] == 0xFFFFFFFFFFFFFFFF)
                    continue;
                
                for(uint8_t bitIndex = 0; bitIndex < 64; bitIndex++)
                {
                    if(!(map->pageMarks[idx][subIndex] & (1 < bitIndex)))
                    {
                        if(!vaddr)
                        {
                            vaddr = ((idx * 32768) + (subIndex * 64)) * ARCH_PAGE_SIZE;
                            amountLeft = amount;
                        }
                        
                        if(!--amountLeft)
                        {
                            MarkPagesIdentity(map, vaddr, amount);
                            return vaddr;
                        }
                    }
                    else
                        vaddr = 0;
                }
            }
        }

        return 0;
    }

    void FreePages(page_map_t* map, uint64_t virt, size_t amount)
    {
        uint64_t index;
        uint64_t subIndex;
        uint8_t bitIndex;

        virt /= ARCH_PAGE_SIZE;
        while(amount)
        {
            index = virt / 32768;
            subIndex = (virt % 32768) / 64;

            if(amount >= 64 && !(virt % 64))
            {
                amount -= 64;
                virt += 64;
                map->pageMarks[index][subIndex] = 0x0;
                continue;
            }

            for(bitIndex = (virt % 64); bitIndex < 64; bitIndex++)
                map->pageMarks[index][subIndex] &= !(1 << bitIndex);
        }
    }

    void MarkPagesIdentity(page_map_t* map, uint64_t virt, size_t amount)
    {
        uint64_t index;
        uint64_t subIndex;
        uint8_t bitIndex;

        virt /= ARCH_PAGE_SIZE;
        while(amount)
        {
            index = virt / 32768;
            subIndex = (virt % 32768) / 64;

            if(amount >= 64 && !(virt % 64))
            {
                amount -= 64;
                virt += 64;
                map->pageMarks[index][subIndex] = 0x0;
                continue;
            }

            for(bitIndex = (virt % 64); bitIndex < 64; bitIndex++)
                map->pageMarks[index][subIndex] |= (1 << bitIndex);
        }
    }

    uintptr_t ConvertVirtToPhys(pml4_t *pml4, uintptr_t addr)
    {
        pml4_entry_t* pml4_entry = &(reinterpret_cast<pml4_entry_t*>(pml4))[Pml4IndexOf(addr)];
        if (!pml4_entry->present)
        {
            return 0;
        }

        pdpt_entry_t* pdptEntry = &(reinterpret_cast<pdpt_entry_t*>(pml4_entry->addr * ARCH_PAGE_SIZE))[PdptIndexOf(addr)];
        if (!pdptEntry->present)
        {
            return 0;
        }

        pd_entry_t* pdEntry = &(reinterpret_cast<pd_entry_t*>(pdptEntry->addr * ARCH_PAGE_SIZE))[PdIndexOf(addr)];
        if (!pdEntry->present)
        {
            return 0;
        }

        page_t* page = &(reinterpret_cast<page_t*>(pdEntry->addr * ARCH_PAGE_SIZE))[PtIndexOf(addr)];

        if (!page->present && !page->addr)
        {
            return 0;
        }

        return (page->addr * ARCH_PAGE_SIZE) + (addr & 0xfff);

        /*
        pml4_entry_t* pml4_entry = &(*pml4[Pml4IndexOf(addr)]);
        if (!pml4_entry->present)
        {
            return 0;
        }

        pdpt_t* pml3 = reinterpret_cast<pdpt_t*>(pml4_entry->addr * ARCH_PAGE_SIZE);
        pdpt_entry_t* pml3_entry = &(*pml3[PdptIndexOf(addr)]);

        if (!pml3_entry->present)
        {
            return 0;
        }

        page_dir_t* pml2 = reinterpret_cast<page_dir_t*>(pml3_entry->addr * ARCH_PAGE_SIZE);
        pd_entry_t* pml2_entry = &(*pml2[PdIndexOf(addr)]);

        if (!pml2_entry->present)
        {
            return 0;
        }

        page_table_t* pml1 = reinterpret_cast<page_table_t*>(pml2_entry->addr * ARCH_PAGE_SIZE);
        page_t* pml1_entry = &(*pml1[PtIndexOf(addr)]);

        if (!pml1_entry->present)
        {
            return 0;
        }

        return (pml1_entry->addr * ARCH_PAGE_SIZE) + (addr & 0xfff);
        */
    }

    void SwitchPml4(pml4_t* pml4) {
        asmi_load_paging((uint64_t)pml4);
    }

    void EnablePaging() {
        SwitchPml4(&kpml4);
    }

    page_map_t* Current() {
        return currentPages;
    }

    pml4_t* KernelPages() {
        return &kpml4;
    }
}