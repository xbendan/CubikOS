#include <Paging.h>

namespace Paging
{
    PML4        pageMapL4       __attribute__((aligned(4096)));
    PML3        pageMapL3       __attribute__((aligned(4096)));
    PML2        pageMapL2       __attribute__((aligned(4096)));
    PML1        pageMapL1[512]  __attribute__((aligned(4096)));

    void Initialize()
    {
        /*
        memset(pageTable4, 0, sizeof(pml4_t));
        memset(pageTable3, 0, sizeof(pdpt_t));
        memset(pageTableHeaps, 0, sizeof(page_dir_t));
        */

        auto &pml4_entry = pageMapL4.entries[0];
        pml4_entry.uaccess = 0;
        pml4_entry.writable = 1;
        pml4_entry.present = 1;
        pml4_entry.addr = (uint64_t)&pageMapL3 / MEM_PAGE_SIZE;

        //PrintNum((uint64_t)&pml4_entry);

        auto &pml3_entry = pageMapL3.entries[0];
        pml3_entry.uaccess = 0;
        pml3_entry.writable = 1;
        pml3_entry.present = 1;
        pml3_entry.addr = (uint64_t)&pageMapL2 / MEM_PAGE_SIZE;

        for (size_t i = 0; i < 512; i++)
        {
            auto &pml2_entry = pageMapL2.entries[i];
            pml2_entry.uaccess = 0;
            pml2_entry.writable = 1;
            pml2_entry.present = 1;
            pml2_entry.addr = (uint64_t)&pageMapL1[i] / MEM_PAGE_SIZE;
        }
        
        __loadPaging(((uintptr_t) &pageMapL4));
        //__enablePaging();
        //Interrupts::RegisterInterruptHandler(14, InterruptHandler_PageFault);
    }
}