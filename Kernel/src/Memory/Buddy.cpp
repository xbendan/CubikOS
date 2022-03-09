#include <stdint.h>
#include <Memory/MemoryAllocation.h>
#include <Memory/Memory.h>
#include <Memory/Buddy.h>
namespace Memory {
    static uint64_t AsFixedSize(unsigned size) {
        size |= size >> 1;
        size |= size >> 2;
        size |= size >> 4;
        size |= size >> 8;
        size |= size >> 16;
        return size + 1;
    }
}