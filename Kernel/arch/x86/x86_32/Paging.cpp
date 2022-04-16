#include <Paging.h>

namespace Arch::x86_32::Paging
{
    __attribute__((aligned(ARCH_PAGE_SIZE))) PageDirectory kpdir      = {};
    __attribute__((aligned(ARCH_PAGE_SIZE))) PageTable     kptab[256] = {};

    void Initialize()
    {
        for (size_t i = 0; i < 256; i++)
        {
            PageDirectoryEntry *entry = &kpdir.entries[i];
            entry->user = 0;
            entry->write = 1;
            entry->present = 1;
            entry->frameNum = (size_t)&kptab[i] / ARCH_PAGE_SIZE;
        }
        
        __loadPaging(((uintptr_t) &kpdir));
        //Interrupts::RegisterInterruptHandler(14, InterruptHandler_PageFault);
    }

    void Enable() { __enablePaging(); }
    void Disable() { __disablePaging(); }
}