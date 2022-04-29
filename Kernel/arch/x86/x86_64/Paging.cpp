#include <Paging.h>

namespace Paging
{
    PML4        kpml4           __attribute__((aligned(4096)));
    PML3        kpml3           __attribute__((aligned(4096)));
    PML2        kpml2[8]        __attribute__((aligned(4096)));
    PML1        kpml1[8][512]   __attribute__((aligned(4096)));

    void InitializeVirtualMemory()
    {
        /*
        memset(pageTable4, 0, sizeof(pml4_t));
        memset(pageTable3, 0, sizeof(pdpt_t));
        memset(pageTableHeaps, 0, sizeof(page_dir_t));
        */

        auto &pml4_entry = kpml4.entries[0];
        pml4_entry.usr = 0;
        pml4_entry.writable = 1;
        pml4_entry.present = 1;
        pml4_entry.addr = (uint64_t)&kpml3 / ARCH_PAGE_SIZE;

        for (size_t var0 = 0; var0 < 8; var++)
        {
            auto &pml3_entry = kpml3.entries[var0];
            pml3_entry.usr = 0;
            pml3_entry.writable = 1;
            pml3_entry.present = 1;
            pml3_entry.addr = (uint64_t)&kpml2[var0] / ARCH_PAGE_SIZE;

            for (size_t var1 = 0; var1 < 512; var1++)
            {
                auto &pml2_entry = kpml2[var0].entries[i];
                pml2_entry.usr = 0;
                pml2_entry.writable = 1;
                pml2_entry.present = 1;
                pml2_entry.addr = (uint64_t)&kpml1[var0][var1] / ARCH_PAGE_SIZE;
            } 
        }
        
        //Interrupts::RegisterInterruptHandler(14, InterruptHandler_PageFault);
    }

    pml4_t* NewVirtualMemoryMap()
    {

    }

    void DestoryVirtualMemoryMap(pml4_t* pml4)
    {

    }

    bool IsVirtualPagePresent(pml4_t* pml4, uintptr_t vaddr)
    {

    }

    void VirtualMemMapAddress(PML4 *pml4, memory_range_t range, uintptr_t vaddr, memory_flags_t flags)
    {
        for (size_t idx = 0; idx < range.size / ARCH_PAGE_SIZE; idx++)
        {
            uintptr_t addr = vaddr + (idx * ARCH_PAGE_SIZE);

            pml4_entry_t* pml4Entry = &pml4->entries[Pml4IndexOf(addr)];
            pdpt_t* pdpt = reinterpret_cast<pdpt_t*>(pml4Entry->addr * ARCH_PAGE_SIZE);
            if(!pml4Entry->present)
            {
                
            }

            pdpt_entry_t* pdptEntry = &pdpt->entries[PdptIndexOf(addr)];
            page_dir_t* pd = reinterpret_cast<page_dir_t*>(pdptEntry->addr * ARCH_PAGE_SIZE);
            if(!pdptEntry->present)
            {

            }

            pd_entry_t* pdEntry = &pd->entries[PdIndexOf(addr)];
            page_table_t* pt = reinterpret_cast<page_table_t*>(pdEntry->addr * ARCH_PAGE_SIZE);
            if(!pdEntry->present)
            {

            }

            pt_entry_t* ptEntry = &pt->entries[PtIndexOf(addr)];

            ptEntry->present = 1;
            ptEntry->writable = 1;
            ptEntry->usr = MEMORY_FLAG_USER & flags;
            ptEntry->addr = (range.base + (idx * ARCH_PAGE_SIZE)) / ARCH_PAGE_SIZE;
        }
    }

    void VirtualMemAlloc(pml4_t* pml4, memory_range_t range, memory_flags_t flags)
    {

    }

    void VirtualMemFree(pml4_t* pml4, memory_range_t range)
    {
        
    }

    uintptr_t ConvertVirtToPhys(PML4 *pml4, uintptr_t vaddr)
    {

    }

    void LoadPml4(pml4_t* pml4)
    {

    }
}