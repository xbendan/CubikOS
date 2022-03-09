#include <Memory.h>

namespace Memory {
    void LoadMemoryInfo(MemoryInfo memInfo)
    {
        if(memInfo->maxSize < 256 * 1024)
        {
            // system panic, memory size is too small
        }
    }

    void InitializeMemoryManagement()
    {
        
    }
}