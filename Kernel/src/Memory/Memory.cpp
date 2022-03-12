#include <Memory.h>
#include <Buddy.h>
#include <Base.h>

namespace Memory {
    void InitializeMemoryManagement()
    {
        if(memInfo.maxSize < 256 * 1024)
        {
            PrintLine("The total memory is too low (<256MB)");
            // system panic, memory size is too small
        }

        for (int i = 0; i < memInfo.mapSize; i++)
        {
            MemoryMapEntry entry = memInfo.entries[i];
            
        }
    }
}