#include <frg/slab.hpp>
#include <GenericArch.h>

struct KernelAllocator
{
    uintptr_t map(size_t len) {
        size_t pageCount = (len + PAGE_SIZE_4K - 1) / PAGE_SIZE_4K;
        uintptr_t ptr = Generic::mm_KernelAllocatePages(pageCount);
        uintptr_t base = ptr;

        while (pageCount--) {
            Memory::KernelMapVirtualMemory4K(Memory::MmBuddyAllocatePages(0), base, 1);
            base += PAGE_SIZE_4K;
        }

        memset(ptr, 0, len);

        return ptr;
    }

    void unmap(uintptr_t addr, size_t len) {
        assert(!(addr & (PAGE_SIZE_4K - 1)));

        size_t pageCount = (len + PAGE_SIZE_4K - 1) / PAGE_SIZE_4K;
        uintptr_t base = addr;

        for (unsigned i = 0; i < pageCount; i++) {
            Memory::FreePages
            Memory::FreePhysicalMemoryBlock(Memory::VirtualToPhysicalAddress(base));
            base += PAGE_SIZE_4K;
        }

        Memory::KernelFree4KPages(reinterpret_cast<void*>(addr), pageCount);
    }

    frg::slab_pool<KernelAllocator, Lock> slabPool{*this};
    frg::slab_allocator<KernelAllocator, Lock> slabAllocator{&slabPool};
}

KernelAllocator* allocator = nullptr;

frg::slab_allocator<KernelAllocator, Lock>& Allocator() {
    // This is a hack to get around the allocator not being initialized
    if (!allocator) {
        size_t pageCount = (sizeof(KernelAllocator) + PAGE_SIZE_4K - 1) / PAGE_SIZE_4K;
        allocator = reinterpret_cast<KernelAllocator*>(Memory::KernelAllocate4KPages(pageCount));
        uintptr_t base = reinterpret_cast<uintptr_t>(allocator);

        while (pageCount--) {
            Memory::KernelMapVirtualMemory4K(Memory::AllocatePhysicalMemoryBlock(), base, 1);
            base += PAGE_SIZE_4K;
        }

        new (allocator) KernelAllocator;
    }

    return allocator->slabAllocator;
}

void* kmalloc(size_t size) { return Allocator().allocate(size); }

void kfree(void* p) { return Allocator().free(p); }

void* krealloc(void* p, size_t sz) { return Allocator().reallocate(p, sz); }