#include <Memory/Memory.h>
#include <Memory/Buddy.h>
#include <Graphic/GraphicsDevice.h>

#define CHECK_RANGE(x)

void* memset(void* addr, int val, size_t count)
{
    uint8_t* xs = (uint8_t*) addr;
    while (count--)
    {
        *xs++ = val;
    }
    return addr;
}

void* memcpy(void* dest, const void* addr, size_t count)
{
    const char* sp = (char*)addr;
    char* dp = (char*)addr;
    for (size_t i = count; i >= sizeof(uint64_t); i = count) {
        *((uint64_t*)dp) = *((uint64_t*)sp);
        sp = sp + sizeof(uint64_t);
        dp = dp + sizeof(uint64_t);
        count -= sizeof(uint64_t);
    }

    for (size_t i = count; i >= 4; i = count) {
        *((uint32_t*)dp) = *((uint32_t*)sp);
        sp = sp + 4;
        dp = dp + 4;
        count -= 4;
    }

    for (size_t i = count; i > 0; i = count) {
        *(dp++) = *(sp++);
        count--;
    }

    return dest;
}

int memcmp(const void* s1, const void* s2, size_t n)
{
    const uint8_t* a = (uint8_t*)s1;
    const uint8_t* b = (uint8_t*)s2;

    for (size_t i = 0; i < n; i++)
    {
        if (*(a + i) != *(b + i))
        {
            return *(a + i) - *(b + i);
        }
    }

    return 0;
}

namespace Memory
{
    uint64_t totalSize;
    system_memory_info_t record;

    void Initialize(uint64_t _totalSize, size_t _mapSize, memory_map_entry_t* _mapEntries)
    {
        totalSize = _totalSize;

        for (int index = 0; index < _mapSize; index++)
        {
            if(_mapEntries[index].type == MEMORY_MAP_ENTRY_AVAILABLE &&
                _mapEntries[index].range.size >= BUDDY_NODE_SIZE)
            {
                Graphics::DrawRect({0,0}, {50, 50}, {127, 127, 127}, 0);

                Memory::Allocation::MmBuddyCreateNode(_mapEntries[index].range);

                /*
                uintptr_t start = _mapEntries[index].range.base;
                uintptr_t end = start + _mapEntries[index].range.size;

                if((__kmstart__ <= start && __kmend__ >= end) || 
                    ((__kmend__ - __kmstart__) >= _mapEntries[index].range.size && (__kmstart__ == start || __kmend__ == end)))
                    continue;
                else if(__kmstart__ > start && __kmend__ < end)
                {
                    Memory::Allocation::MmBuddyCreateNode(start, __kmstart__  - 1);
                    Memory::Allocation::MmBuddyCreateNode(__kmend__ + 1, end);
                    continue;
                }
                else if(__kmstart__ == start && __kmend__ < end)
                    start += (__kmend__ - __kmstart__);
                else if(__kmstart__ > start && __kmend__ == end)
                    end -= (__kmend__ - __kmstart__);
                

                Memory::Allocation::MmBuddyCreateNode(start, end);
                */
            }
        }

        //Memory::Allocation::MmMarkRangeUsed((uintptr_t)__kmstart__, (size_t)(__kmend__ - __kmstart__));
    }

    memory_range_t KernelMemoryRange()
    {
        return MemoryRange::AroundNonAlignedStartEnd(__kmstart__, __kmend__);
    }
}