#pragma once

#include <utils/range.h>
#include <macros.h>
#define MEMORY_MAP_LIMIT 64

enum memory_map_entry_type
{
    MemoryMapEntryTypeAvailable = 0,
    MemoryMapEntryTypeReserved = 1,
    MemoryMapEntryTypeAcpiReclaimable = 2,
    MemoryMapEntryTypeNvs = 3,
    MemoryMapEntryTypeBadRam = 4,
    MemoryMapEntryTypeKernel = 5
};

struct memory_map_entry
{
    range_t range;
    enum memory_map_entry_type type;
};

struct boot_mem
{
    uint64_t total_size;
    uint64_t usable;
    size_t map_size;
    struct memory_map_entry entries[MEMORY_MAP_LIMIT];
};
struct boot_graphic
{
    struct {

    } framebuffer;
};

typedef struct boot_info
{
    uint64_t check;
    const char* bl_name;
    struct boot_mem mem;
    struct boot_graphic graphic;
} boot_info_t;

boot_info_t* get_boot_info();