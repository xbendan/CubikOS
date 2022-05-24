#include <GenericArch.h>

namespace Generic
{
    uintptr_t mm_ConvertVirtToPhys(pml4_t* pml4, uintptr_t virt) { ConvertVirtToPhys(pml4, virt); }
    page_map_t* mm_CreateVirtualMemoryMap() { CreateVirtualMemoryMap(); }
    void mm_DestoryVirtualMemoryMap(page_map_t* map) { DestoryVirtualMemoryMap(map); }
    void mm_KernelMapVirtualAddress(uintptr_t phys, uintptr_t virt, size_t amount) { KernelMapVirtualAddress(phys, virt, amount); }
    uintptr_t mm_KernelAllocatePages(size_t amount) { KernelAllocatePages(amount); }
    void mm_KernelFreePages(uintptr_t virt, size_t amount) { KernelFreePages(virt, amount); }
    void mm_MapVirtualAddress(page_map_t* map, uintptr_t phys, uintptr_t virt, size_t amount) { MapVirtualAddress(map, phys, virt, amount); }
    uintptr_t mm_AllocatePages(page_map_t* map, size_t amount) { AllocatePages(map, amount); }
    void mm_FreePages(page_map_t* map, uintptr_t virt, size_t amount) { FreePages(map, virt, amount); }
}