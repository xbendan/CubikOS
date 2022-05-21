namespace Generic
{
    uintptr_t mm_ConvertPhysToVirt(uintptr_t phys);
    uintptr_t mm_KernelMapVirtualAddress(uintptr_t phys, uintptr_t virt, size_t amount);
    uintptr_t mm_MapVirtualAddress(uintptr_t phys, uintptr_t virt, size_t amount);
}