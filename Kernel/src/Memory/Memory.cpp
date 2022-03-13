#include <Memory.h>

extern "C" void* memset(void* src, int c, size_t count) {
    uint8_t* xs = (uint8_t*)src;

    while (count--)
        *xs++ = c;

    return src;
}

extern "C" void* memcpy(void* dest, const void* src, size_t count) {
    const char* sp = (char*)src;
    char* dp = (char*)dest;
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

extern "C" int memcmp(const void* s1, const void* s2, size_t n) {
    const uint8_t* a = (uint8_t*)s1;
    const uint8_t* b = (uint8_t*)s2;

    for (size_t i = 0; i < n; i++) {
        if (a[i] < b[i]) {
            return -1;
        } else if (a[i] > b[i]) {
            return 1;
        }
    }

    return 0;
}

namespace Memory {
    memory_info_t memInfo;

    memory_info_t* _memInfo()
    {
        return &memInfo;
    }

    void InitializeMemoryManagement()
    {
        if(memInfo.total < 256 * 1024)
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