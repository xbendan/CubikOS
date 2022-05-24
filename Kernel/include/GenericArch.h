#include <stdint.h>
#include <stddef.h>
#include <x86/x86_64/Paging.h>

using namespace Paging;

namespace Generic
{
    uintptr_t mm_ConvertVirtToPhys(uintptr_t phys);
    page_map_t* mm_CreateVirtualMemoryMap();
    void mm_DestoryVirtualMemoryMap(page_map_t* map);
    void mm_KernelMapVirtualAddress(uintptr_t phys, uintptr_t virt, size_t amount);
    uintptr_t mm_KernelAllocatePages(size_t amount);
    void mm_KernelFreePages(uintptr_t virt, size_t amount);
    void mm_MapVirtualAddress(page_map_t* map, uintptr_t phys, uintptr_t virt, size_t amount);
    uintptr_t mm_AllocatePages(page_map_t* map, size_t amount);
    void mm_FreePages(page_map_t* map, uintptr_t virt, size_t amount);
}