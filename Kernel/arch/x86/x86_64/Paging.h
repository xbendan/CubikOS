#pragma once

#include <stdint.h>
#include <stddef.h>
#include <MemoryFlags.h>
#include <MemoryRange.h>

#define ARCH_PAGE_SIZE      (4096)
#define PAGES_PER_TABLE     512
#define TABLES_PER_DIR      512
#define DIRS_PER_PDPT       512
#define PDPTS_PER_PML4      512
#define PML4_INDEX_OF(addr) (((addr) >> 39) & 0x1FF)
#define PDPT_INDEX_OF(addr) (((addr) >> 30) & 0x1FF)
#define PAGE_DIR_INDEX_OF(addr) (((addr) >> 21) & 0x1FF)
#define PAGE_TABLE_INDEX_OF(addr) (((addr) >> 12) & 0x1FF)
#define SIZE_TO_PAGE(size) (size / ARCH_PAGE_SIZE)
#define IS_PAGE_ALIGNED(addr) (addr % ARCH_PAGE_SIZE == 0)

using namespace Memory;

namespace Paging {
    typedef struct PML4Entry {
        bool present: 1; // Must be 1
        bool writable: 1; // Page is readonly if set to 0, also called Read/Write bit
        bool usr: 1; // Everyone could access this page if it's not 0, or only supervisor allowed.
        bool writeThrough: 1; // Write-Through cache is enabled when this bit is set
        bool cache: 1; // Disable cache if it's set to 1
        bool access: 1; // Is this page entry has been used.
        int ign6_11: 6;
        uint64_t addr: 36; // Physical Address
        int ign48_62 : 15;                 // Ignored
        bool disableExecution : 1;
    } __attribute__((packed)) pml4_entry_t;

    typedef struct PML3Entry {
        bool present: 1; // Must be 1
        bool writable: 1; // Page is readonly if set to 0, also called Read/Write bit
        bool usr: 1; // Everyone could access this page if it's not 0, or only supervisor allowed.
        bool writeThrough: 1; // Write-Through cache is enabled when this bit is set
        bool cache: 1; // Disable cache if it's set to 1
        bool access: 1; // Is this page entry has been used.
        int ign6: 1;
        int size: 1;
        int ign8_11: 4;
        uint64_t addr: 36;
        int ign48_62: 15;
        bool disableExecution: 1;
    } __attribute__((packed)) pdpt_entry_t;

    typedef struct PML2Entry {
        bool present: 1; // Must be 1
        bool writable: 1; // Page is readonly if set to 0, also called Read/Write bit
        bool usr: 1; // Everyone could access this page if it's not 0, or only supervisor allowed.
        bool writeThrough: 1; // Write-Through cache is enabled when this bit is set
        bool cache: 1; // Disable cache if it's set to 1
        bool access: 1; // Is this page entry has been used.
        int ign6: 1;
        int size: 1;
        int ign8_11: 4;
        uint64_t addr: 36;
        int ign48_62: 15;
        bool disableExecution: 1;
    } __attribute__((packed)) pd_entry_t;

    typedef struct PML1Entry {
        bool present: 1; // Must be 1
        bool writable: 1; // Page is readonly if set to 0, also called Read/Write bit
        bool usr: 1; // Everyone could access this page if it's not 0, or only supervisor allowed.
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
    } __attribute__((packed)) pt_entry_t;

    typedef struct PML4
    {
        PML4Entry entries[PDPTS_PER_PML4];
    } __attribute__((packed)) pml4_t;

    typedef struct PML3
    {
        PML3Entry entries[DIRS_PER_PDPT];
    } __attribute__((packed)) pdpt_t;

    typedef struct PML2
    {
        PML2Entry entries[TABLES_PER_DIR];
    } __attribute__((packed)) page_dir_t;

    typedef struct PML1
    {
        PML1Entry entries[PAGES_PER_TABLE];
    } __attribute__((packed)) page_table_t;

    extern "C" void __loadPaging(uintptr_t addr);
    extern "C" void __enablePaging();
    extern "C" void __disablePaging();

    static inline size_t Pml4IndexOf(uintptr_t addr) { return (addr >> 39) & 0x1FF; }
    static inline size_t PdptIndexOf(uintptr_t addr) { return (addr >> 30) & 0x1FF; }
    static inline size_t PdIndexOf(uintptr_t addr) { return (addr >> 21) & 0x1FF; }
    static inline size_t PtIndexOf(uintptr_t addr) { return (addr >> 12) & 0x1FF; }

    void InitializeVirtualMemory();
    pml4_t* NewVirtualMemoryMap();
    void DestoryVirtualMemoryMap(pml4_t* pml4);
    bool IsVirtualPagePresent(pml4_t* pml4, uintptr_t vaddr);
    void VirtualMemMapAddress(pml4_t* pml4, memory_range_t range, uintptr_t vaddr, memory_flags_t flags);
    void VirtualMemAlloc(pml4_t* pml4, memory_range_t range, memory_flags_t flags);
    void VirtualMemFree(pml4_t* pml4, memory_range_t range);
    uintptr_t ConvertVirtToPhys(pml4_t* pml4, uintptr_t vaddr);
    void LoadPml4(pml4_t* pml4);
}