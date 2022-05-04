#include <Paging.h>
#include <Buddy.h>
#include <GraphicsDevice.h>

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

    pml4_t* CreateVirtualMemoryMap()
    {

    }

    void DestoryVirtualMemoryMap(pml4_t* pml4)
    {

    }

    bool IsVirtualPagePresent(pml4_t* pml4, uintptr_t vaddr)
    {

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

    void MapVirtualMemoryAddress(pml4_t *pml4, uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags)
    {
        uint64_t pml4Index, pdptIndex, pdIndex, pageIndex;

        while(amount--)
        {
            pml4Index = Pml4IndexOf(virt);
            pdptIndex = PdptIndexOf(virt);
            pdIndex = PdIndexOf(virt);
            pageIndex = PtIndexOf(virt);


        }
        /*
        for (size_t idx = 0; idx < amount; idx++)
        {
            uintptr_t addr = vaddr + (idx * ARCH_PAGE_SIZE);

            pml4_entry_t* pml4Entry = &(*pml4[Pml4IndexOf(addr)]);
            pdpt_t* pdpt = reinterpret_cast<pdpt_t*>(pml4Entry->addr * ARCH_PAGE_SIZE);
            if(!pml4Entry->present)
            {
                
            }

            pdpt_entry_t* pdptEntry = &(*pdpt[PdptIndexOf(addr)]);
            page_dir_t* pd = reinterpret_cast<page_dir_t*>(pdptEntry->addr * ARCH_PAGE_SIZE);
            if(!pdptEntry->present)
            {

            }

            pd_entry_t* pdEntry = &(*pd[PdIndexOf(addr)]);
            page_table_t* pt = reinterpret_cast<page_table_t*>(pdEntry->addr * ARCH_PAGE_SIZE);
            if(!pdEntry->present)
            {

            }

            page_t* pageEntry = &(*pt[PtIndexOf(addr)]);

            pageEntry->present = 1;
            pageEntry->writable = 1;
            pageEntry->usr = MEMORY_FLAG_USER & flags;
            pageEntry->addr = (phys + (idx * ARCH_PAGE_SIZE)) / ARCH_PAGE_SIZE;
        }
        */
    }

    void MapVirtualMemoryAddress(pml4_t *pml4, uint64_t phys, uint64_t virt, size_t amount){
        MapVirtualMemoryAddress(pml4, phys, virt, amount, PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE | PAGE_FLAG_USER);
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