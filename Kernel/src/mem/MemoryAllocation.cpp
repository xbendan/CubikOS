#include <GenericArch.h>
#include <mem/Buddy.h>
#include <mem/Slub.h>
#include <mem/MemoryAllocation.h>

namespace Memory
{
    uintptr_t Allocate(size_t size)
    {
        return AllocatePages(ToPowerOf2(size) / ARCH_PAGE_SIZE);
    }

    uintptr_t AllocatePages(size_t amount)
    {
        amount = ToPowerOf2(amount);
        
        uintptr_t phys = MmBuddyAllocatePages(ToOrder(amount))->addr;
        uintptr_t virt = Generic::mm_KernelAllocatePages(amount);

        if(phys || virt)
            return 0;
        
        Generic::mm_KernelMapVirtualAddress(phys, virt, amount);
        return virt;
    }
}