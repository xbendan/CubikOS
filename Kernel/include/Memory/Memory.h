#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#define MEM_PAGE_SIZE       4096
#define PAGES_PER_TABLE     512
#define TABLES_PER_DIR      512
#define DIRS_PER_PDPT       512
#define PDPTS_PER_PML4      512
#define MEMORY_MAP_LIMIT    64;
#define PML4_INDEX_OF(addr) (((addr) >> 39) & 0x1FF)
#define PDPT_INDEX_OF(addr) (((addr) >> 30) & 0x1FF)
#define PAGE_DIR_INDEX_OF(addr) (((addr) >> 21) & 0x1FF)
#define PAGE_TABLE_INDEX_OF(addr) (((addr) >> 12) & 0x1FF)
#define SIZE_TO_PAGE(size) (size / MEM_PAGE_SIZE)
#define IS_PAGE_ALIGNED(addr) (addr % MEM_PAGE_SIZE == 0)

extern "C" void* memset(void* src, int c, size_t count);
extern "C" void* memcpy(void* dest, const void* src, size_t count);
extern "C" int memcmp(const void* s1, const void* s2, size_t n);
extern "C" void LoadPageMaps(uintptr_t ptr);

namespace Memory {
    MemoryInfo memInfo;

    typedef struct MemoryInfo {
        size_t total;
        size_t usable;
        MemoryMapEntry entries[MEMORY_MAP_LIMIT];
        size_t mapSize;
    } memory_info_t;

    struct MemoryMapEntry {
        MemoryBlock block {};
        MemoryMapEntryType type;
    };

    enum MemoryMapEntryType
    {
        MEMORY_MAP_ENTRY_AVAILABLE,
        MEMORY_MAP_ENTRY_RESERVED,
        MEMORY_MAP_ENTRY_ACPI_RECLAIMABLE,
        MEMORY_MAP_ENTRY_NVS,
        MEMORY_MAP_ENTRY_BADRAM,
        MEMORY_MAP_ENTRY_KERNEL
    };
    
    typedef struct PML4Entry {
        bool present: 1; // Must be 1
        bool writable: 1; // Page is readonly if set to 0, also called Read/Write bit
        bool uaccess: 1; // Everyone could access this page if it's not 0, or only supervisor allowed.
        bool writeThrough: 1; // Write-Through cache is enabled when this bit is set
        bool cache: 1; // Disable cache if it's set to 1
        bool access: 1; // Is this page entry has been used.
        int ign6_11: 6;
        uint64_t addr: 36; // Physical Address
        int ign48_62 : 15;                 // Ignored
        bool execute_disabled : 1;
    } __attribute__((packed)) pml4_entry_t;

    typedef struct PML3Entry {
        bool present: 1; // Must be 1
        bool writable: 1; // Page is readonly if set to 0, also called Read/Write bit
        bool uaccess: 1; // Everyone could access this page if it's not 0, or only supervisor allowed.
        bool writeThrough: 1; // Write-Through cache is enabled when this bit is set
        bool cache: 1; // Disable cache if it's set to 1
        bool access: 1; // Is this page entry has been used.
        int ign6: 1;
        int size: 1;
        int ign8_11: 4;
        uint64_t addr: 36;
        int ign48_62: 15;
        bool execute_disabled: 1;
    } __attribute__((packed)) pml3_entry_t;

    typedef struct PML2Entry {
        bool present: 1; // Must be 1
        bool writable: 1; // Page is readonly if set to 0, also called Read/Write bit
        bool uaccess: 1; // Everyone could access this page if it's not 0, or only supervisor allowed.
        bool writeThrough: 1; // Write-Through cache is enabled when this bit is set
        bool cache: 1; // Disable cache if it's set to 1
        bool access: 1; // Is this page entry has been used.
        int ign6: 1;
        int size: 1;
        int ign8_11: 4;
        uint64_t addr: 36;
        int ign48_62: 15;
        bool execute_disabled: 1;
    } __attribute__((packed)) pml2_entry_t;

    typedef struct PML1Entry {
        bool present: 1; // Must be 1
        bool writable: 1; // Page is readonly if set to 0, also called Read/Write bit
        bool uaccess: 1; // Everyone could access this page if it's not 0, or only supervisor allowed.
        bool writeThrough: 1; // Write-Through cache is enabled when this bit is set
        bool cache: 1; // Disable cache if it's set to 1
        bool access: 1; // Is this page entry has been used.
        int dirty: 1;
        int memType: 1;
        int global: 1;
        int ign9_11: 3;
        uint64_t addr: 36;
        int ign48_57: 10;
        bool key: 5;
        bool disableExecution: 1;
    } __attribute__((packed)) pml1_entry_t;

    typedef struct MemoryBlock {
        uintptr_t base;
        size_t size;
    } memory_range_t;
    
    using page_table_t = pml1_entry_t[PAGES_PER_TABLE]
    using page_dir_t = pml2_entry_t[TABLES_PER_DIR];
    using pdpt_t = = pml3_entry_t[DIRS_PER_PDPT];
    using pml4_t = pml4_entry_t[PDPTS_PER_PML4];

    void InitializeMemoryManagement(MemoryInfo memInfo);
    void FreePhysicalPage(uintptr_t addr);
    void FreePhysicalBlock(memory_range_t block);
}