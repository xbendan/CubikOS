namespace Generic
{
    uintptr_t mm_ConvertPhysToVirt(uintptr_t phys);
    page_map_t* mm_CreateVirtualMemoryMap();
    void mm_DestoryVirtualMemoryMap(page_map_t* map);
    void mm_KernelMapVirtualAddress(uintptr_t phys, uintptr_t virt, size_t amount);
    uintptr_t mm_KernelAllocatePages(size_t amount);
    void mm_KernelFreePages(uintptr_t virt, size_t amount);
    void mm_MapVirtualAddress(uintptr_t phys, uintptr_t virt, size_t amount);
    uintptr_t mm_AllocatePages(size_t amount);
    void mm_FreePages(uintptr_t virt, size_t amount);
}