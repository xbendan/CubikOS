#include <Memory.h>
#include <Buddy.h>

extern uintptr_t __kmstart__;
extern uintptr_t __kmend__;

namespace Memory
{
    uint64_t totalSize;

    void Initialize(uint64_t _totalSize, size_t _mapSize, memory_map_entry_t* _mapEntries)
    {
        totalSize = _totalSize;
        for (int index = 0; index < _mapSize; index++)
        {
            if(_mapEntries[index].type == MEMORY_MAP_ENTRY_AVAILABLE)
            {
                uint64_t base = _mapEntries[index].range.base;
                uint64_t size = _mapEntries[index].range.size;
                while (size >= BUDDY_NODE_SIZE)
                {
                    Memory::Allocation::BdCreateNode(base);
                    base += BUDDY_NODE_SIZE;
                    size -= BUDDY_NODE_SIZE;
                }
            }
        }

        //Memory::Allocation::BdMarkRangeUsed(__kmstart__, (size_t)(__kmend__ - __kmstart__));
    }
}