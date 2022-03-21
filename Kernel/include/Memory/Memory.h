#pragma once

#include <stdint.h>
#include <stddef.h>
#define MEMORY_MAP_LIMIT    64
#ifndef ARCH_PAGE_SIZE
#define ARCH_PAGE_SIZE (4096)
#endif

extern "C" void* memset(void* src, int c, size_t count);
extern "C" void* memcpy(void* dest, const void* src, size_t count);
extern "C" int memcmp(const void* s1, const void* s2, size_t n);

extern "C" void LoadPageMaps(uintptr_t ptr);

typedef struct MemoryInfo
{
    /* data */
} memory_info_t;


namespace Memory {
    typedef struct MemoryRange {
        uint64_t base;
        uint64_t size;
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

    void InitializeMemoryManagement();
    void FreePhysicalPage(uintptr_t addr);
    void FreePhysicalBlock(memory_range_t block);
}