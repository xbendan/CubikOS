#pragma once

#include <stdint.h>
#include <stddef.h>
#include <MemoryFlags.h>
#include <MemoryRange.h>
#include <Memory.h>

#define ARCH_PAGE_SIZE      (4096)
#define PAGES_PER_TABLE     512
#define TABLES_PER_DIR      512
#define DIRS_PER_PDPT       512
#define PDPTS_PER_PML4      512
#define PML4_INDEX_OF(addr) (((addr) >> 39) & 0x1FF)
#define PDPT_INDEX_OF(addr) (((addr) >> 30) & 0x1FF)
#define PAGE_DIR_INDEX_OF(addr) (((addr) >> 21) & 0x1FF)
#define PAGE_TABLE_INDEX_OF(addr) (((addr) >> 12) & 0x1FF)
#define PAGE_FLAG_PRESENT       (1 << 0)
#define PAGE_FLAG_WRITABLE      (1 << 1)
#define PAGE_FLAG_USER          (1 << 2)
#define PAGE_FLAG_WRITETHROUGH  (1 << 3)
#define PAGE_FLAG_NOCACHE       (1 << 4)
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
    } __attribute__((packed)) page_t;

    typedef uint64_t page_flags_t;

    using pml4_t = pml4_entry_t[PDPTS_PER_PML4]; /* 512GiB -> 256TiB */
    using pdpt_t = pdpt_entry_t[DIRS_PER_PDPT]; /* 1GiB -> 512GiB */
    using page_dir_t = pd_entry_t[TABLES_PER_DIR]; /* 2MiB -> 1GiB */
    using page_table_t = page_t[PAGES_PER_TABLE]; /* 4KiB -> 2MiB */

    static inline void asmi_load_paging(uintptr_t addr) { asm("mov %%rax, %%cr3" ::"a"((uint64_t)addr)); }
    static inline void asmi_invlpg(uintptr_t addr) { asm volatile("invlpg (%0)"::"r"(addr)); }
    static inline size_t Pml4IndexOf(uintptr_t addr) { return (addr >> 39) & 0x1FF; }
    static inline size_t PdptIndexOf(uintptr_t addr) { return (addr >> 30) & 0x1FF; }
    static inline size_t PdIndexOf(uintptr_t addr) { return (addr >> 21) & 0x1FF; }
    static inline size_t PtIndexOf(uintptr_t addr) { return (addr >> 12) & 0x1FF; }

    void InitializeVirtualMemory();
    pml4_t* CreateVirtualMemoryMap();
    void DestoryVirtualMemoryMap(pml4_t* pml4);
    bool IsVirtualPagePresent(pml4_t* pml4, uintptr_t vaddr);
    void KernelMapVirtualMemoryAddress(uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags);
    void KernelMapVirtualMemoryAddress(uint64_t phys, uint64_t virt, size_t amount);
    void MapVirtualMemoryAddress(pml4_t* pml4, uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags);
    void MapVirtualMemoryAddress(pml4_t* pml4, uint64_t phys, uint64_t virt, size_t amount);
    void AllocatePages(pml4_t* pml4, uint64_t phys, size_t amount, page_flags_t flags);
    void AllocatePages(pml4_t* pml4, size_t amount, page_flags_t flags);
    void FreePages(pml4_t* pml4, uint64_t virt, size_t amount);
    uintptr_t ConvertVirtToPhys(pml4_t* pml4, uintptr_t vaddr);
    void SwitchPml4(pml4_t* pml4);
}