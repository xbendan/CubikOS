#include <Paging.h>

namespace Arch::x86_64::Paging
{
    PML4        kpml4       __attribute__((aligned(4096)));
    PML3        kpml3       __attribute__((aligned(4096)));
    PML2        kpml2       __attribute__((aligned(4096)));
    PML1        kpml1[512]  __attribute__((aligned(4096)));

    void Initialize()
    {
        /*
        memset(pageTable4, 0, sizeof(pml4_t));
        memset(pageTable3, 0, sizeof(pdpt_t));
        memset(pageTableHeaps, 0, sizeof(page_dir_t));
        */

        auto &pml4_entry = kpml4.entries[0];
        pml4_entry.uaccess = 0;
        pml4_entry.writable = 1;
        pml4_entry.present = 1;
        pml4_entry.addr = (uint64_t)&kpml3 / ARCH_PAGE_SIZE;

        //PrintNum((uint64_t)&pml4_entry);

        auto &pml3_entry = kpml3.entries[0];
        pml3_entry.uaccess = 0;
        pml3_entry.writable = 1;
        pml3_entry.present = 1;
        pml3_entry.addr = (uint64_t)&kpml2 / ARCH_PAGE_SIZE;

        for (size_t i = 0; i < 512; i++)
        {
            auto &pml2_entry = kpml2.entries[i];
            pml2_entry.uaccess = 0;
            pml2_entry.writable = 1;
            pml2_entry.present = 1;
            pml2_entry.addr = (uint64_t)&kpml1[i] / ARCH_PAGE_SIZE;
        }
        //Interrupts::RegisterInterruptHandler(14, InterruptHandler_PageFault);
    }

    uintptr_t ConvertVirtToPhys(PML4 *pml4, uintptr_t vaddr)
    {

    }
}