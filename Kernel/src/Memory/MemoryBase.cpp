#include <Memory.h>
#include <Buddy.h>
#include <Math.h>
#include <GraphicsDevice.h>

extern uintptr_t __kmstart__;
extern uintptr_t __kmend__;

namespace Memory
{
    uint64_t totalSize;
    memory_record_t memoryRecord;

    void Initialize(uint64_t _totalSize, size_t _mapSize, memory_map_entry_t* _mapEntries)
    {
        totalSize = _totalSize;
        for (int index = 0; index < _mapSize; index++)
        {
            if(_mapEntries[index].type == MEMORY_MAP_ENTRY_AVAILABLE)
            {
                if (_mapEntries[index].range.size >= BUDDY_NODE_SIZE)
                {
                    Memory::Allocation::BuddyCreateNode(_mapEntries[index].range);
                }
            }
        }

        uint8_t val = Math::log2(512);
        Graphics::DrawRect(
            (Point){50, 50},
            (Size){200, 50},
            (PixelColor){val, val, val},
            0
        );

        Memory::Allocation::BuddyMarkRangeUsed((uintptr_t)__kmstart__, (size_t)(__kmend__ - __kmstart__));
    }
}