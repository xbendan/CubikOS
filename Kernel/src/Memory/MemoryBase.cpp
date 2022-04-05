#include <Memory.h>
#include <Buddy.h>
#include <GraphicsDevice.h>

#define CHECK_RANGE(x)

extern uintptr_t __kmstart__;
extern uintptr_t __kmend__;

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

                uintptr_t start = _mapEntries[index].range.base;
                uintptr_t end = start + _mapEntries[index].range.size;

                if((__kmstart__ <= start && __kmend__ >= end) || 
                    ((__kmend__ - __kmstart__) >= _mapEntries[index].range.size && (__kmstart__ == start || __kmend__ == end)))
                    continue;
                else if(__kmstart__ > start && __kmend__ < end)
                {
                    Memory::Allocation::MmCreateNode(start, __kmstart__  - 1);
                    Memory::Allocation::MmCreateNode(__kmend__ + 1, end);
                    continue;
                }
                else if(__kmstart__ == start && __kmend__ < end)
                    start += (__kmend__ - __kmstart__);
                else if(__kmstart__ > start && __kmend__ == end)
                    end -= (__kmend__ - __kmstart__);

                Memory::Allocation::MmCreateNode(start, end);
            }
        }

        Memory::Allocation::MmCreateNode((uintptr_t)__kmstart__, (size_t)(__kmend__ - __kmstart__));
    }
}