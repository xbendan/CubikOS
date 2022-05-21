#include <GenericArch.h>
#include <Paging.h>

namespace Generic
{
    uintptr_t mm_ConvertPhysToVirt(uintptr_t phys) { Paging::ConvertPhysToVirt(phys); }
    page_map_t* mm_CreateVirtualMemoryMap() { Paging::CreateVirtualMemoryMap(); }
    void mm_DestoryVirtualMemoryMap(page_map_t* map) { Paging::DestoryVirtualMemoryMap(map); }
    void mm_KernelMapVirtualAddress(uintptr_t phys, uintptr_t virt, size_t amount) { Paging::KernelMapVirtualAddress(phys, virt, amount); }
    uintptr_t mm_KernelAllocatePages(size_t amount) { Paging::KernelAllocatePages(amount); }
    void mm_KernelFreePages(uintptr_t virt, size_t amount) { Paging::KernelFreePages(virt, amount); }
    void mm_MapVirtualAddress(uintptr_t phys, uintptr_t virt, size_t amount) { Paging::MapVirtualAddress(phys, virt, amount); }
    uintptr_t mm_AllocatePages(size_t amount) { Paging::AllocatePages(amount); }
    void mm_FreePages(uintptr_t virt, size_t amount) { Paging::FreePages(virt, amount); }
}