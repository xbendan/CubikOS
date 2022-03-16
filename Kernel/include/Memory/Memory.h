#include <Base.h>
#define MEMORY_MAP_LIMIT    64


extern "C" void* memset(void* src, int c, size_t count);
extern "C" void* memcpy(void* dest, const void* src, size_t count);
extern "C" int memcmp(const void* s1, const void* s2, size_t n);
extern "C" void LoadPageMaps(uintptr_t ptr);

namespace Memory {
    typedef struct MemoryRange {
        uintptr_t base;
        size_t size;
    } memory_range_t;

    enum MemoryMapEntryType
    {
        MEMORY_MAP_ENTRY_AVAILABLE,
        MEMORY_MAP_ENTRY_RESERVED,
        MEMORY_MAP_ENTRY_ACPI_RECLAIMABLE,
        MEMORY_MAP_ENTRY_NVS,
        MEMORY_MAP_ENTRY_BADRAM,
        MEMORY_MAP_ENTRY_KERNEL
    };

    typedef struct MemoryMapEntry {
        MemoryRange range {};
        MemoryMapEntryType type;
    } memory_map_entry_t;

    typedef struct MemoryInfo {
        size_t total;
        size_t usable;
        memory_map_entry_t entries[MEMORY_MAP_LIMIT];
        size_t mapSize;
    } memory_info_t;

    memory_info_t* _memInfo();
    void InitializeMemoryManagement();
    void FreePhysicalPage(uintptr_t addr);
    void FreePhysicalBlock(memory_range_t block);
}