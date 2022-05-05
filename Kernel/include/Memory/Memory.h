/**
 * @file Memory.h
 * @author RainbowMeowCat ()
 * @brief 
 * @version 0.1
 * @date 2022-03-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef HEADER_FILE_MEMORY
#define HEADER_FILE_MEMORY

#include <stdint.h>
#include <stddef.h>
#include <MemoryRange.h>
#define MEMORY_MAP_LIMIT    64
#ifndef ARCH_PAGE_SIZE
#define ARCH_PAGE_SIZE (4096)
#endif

extern uint32_t __kmstart__;
extern uint32_t __kmend__;

void* memset(void* addr, int val, size_t count);
void* memcpy(void* dest, const void* addr, size_t count);
int memcmp(const void* s1, const void* s2, size_t n);

extern "C" void LoadPageMaps(uintptr_t ptr);

typedef struct MemoryInfo
{
    /* data */
} memory_info_t;

/**
 * This struct represents the general information about memory management
 */
typedef struct SystemMemoryInfo
{
    uint64_t totalSize; /* How many physical memory does the computer have */
    uint64_t available; /* How many physical memory is usable */
    size_t inUsed; /* The memory that is actually being used */
    size_t allocated; /* The size that has been allocated. No less than inUsed! */
    size_t standby; /* Memory that frozen to save physical memory, saved in disk */
    size_t free; /* Memory that is free and can be allocated */
} system_memory_info_t;

/**
 * @brief The simplified struct of @see SystemMemoryInfo
 * This struct represents the memory related to a specific process.
 */
typedef struct ProcessMemoryRecord
{
    /**
     * @brief Indicates how many memory is actually in used
     */
    size_t inUsed;
    /**
     * @brief Indicates how many memory is allocated
     * This value should always be equals to or greater
     * than @code size_t inUsed @endcode
     * In other words, this value is how many memory that the process
     * got from the allocator. But it doesn't mean that the process
     * knows it got such a lot memory. The process always considers that
     * it has @code size_t inUsed @endcode KiB memory.
     * 
     * For instance, once the process asks for 41KiB memory, but the
     * allocator decided to allocate 44KiB memory. The value of "inUsed"
     * is 41KiB and the value of "allocated" is 44KiB
     */
    size_t allocated;
} memory_record_t;

namespace Memory {
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
        memory_range_t range {};
        MemoryMapEntryType type;
    } memory_map_entry_t;


    typedef void (*paging_t)(void);

    /**
     * @brief Initialize the memory management
     * 
     * 
     * 
     * @param totalSize The total size of system memory, including the unusable region.
     * @param mapSize indicates the size of mapEntries
     * @param mapEntries storage the entries for initialize
     */
    void Initialize(uint64_t totalSize, size_t mapSize, memory_map_entry_t* mapEntries);
    memory_range_t KernelMemoryRange();
}

void* kmalloc(size_t size);
void kfree(void* p);

#endif