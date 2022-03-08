#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

namespace Memory {

    void InitPhysicalAllocator(MemoryInfo memInfo);

    uint64_t RequestMemoryBlockAllocation(size_t size);
    void FreeMemoryBlock(uint64_t base, size_t size);
}