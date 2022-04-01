#include <Memory.h>
#include <Buddy.h>
#include <GraphicsDevice.h>

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
            if(_mapEntries[index].type == MEMORY_MAP_ENTRY_AVAILABLE)
            {
                if (_mapEntries[index].range.size >= BUDDY_NODE_SIZE)
                {
                    Graphics::DrawRect({0,0}, {50, 50}, {127, 127, 127}, 0);
                    Memory::Allocation::BuddyCreateNode(_mapEntries[index].range);
                }
            }
        }

        Memory::Allocation::BuddyMarkRangeUsed((uintptr_t)__kmstart__, (size_t)(__kmend__ - __kmstart__));
    }
}