#include <utils/range.h>
#include <macros.h>
#define MEMORY_MAP_LIMIT 64

enum memory_map_entry_type
{
    MemoryMapEntryTypeAvailable,
    MemoryMapEntryTypeReserved,
    MemoryMapEntryTypeAcpiReclaimable,
    MemoryMapEntryTypeNvs,
    MemoryMapEntryTypeBadRam,
    MemoryMapEntryTypeKernel
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

struct boot_graphic_framebuffer
{
    
};


struct boot_graphic
{
    
};

typedef struct boot_info
{
    uint64_t check;
    const char* bl_name;
    struct boot_mem mem;
    struct boot_graphic graphic;
} boot_info_t;