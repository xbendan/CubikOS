#include <GenericArch.h>
#include <Buddy.h>
#include <Slub.h>
#include <Memory/MemoryAllocation.h>

namespace Memory
{
    uintptr_t Allocate(size_t size)
    {

    }

    uintptr_t AllocatePages(size_t amount)
    {
        uintptr_t phys = MmBuddyAllocatePages(amount);
        uintptr_t virt = mm_KernelAllocatePages(amount);

        if(phys || virt)
            return 0;
        
        mm_KernelMapVirtualAddress(phys, virt, amount);
        return virt;
    }
}