#include <macros.h>
#include <mem/mflags.h>
#include <mem/memory.h>
#include <utils/range.h>

#define ARCH_PAGE_SIZE              (4096)
#define PAGES_PER_TABLE             512
#define TABLES_PER_DIR              512
#define DIRS_PER_PDPT               512
#define PDPTS_PER_PML4              512
#define PML4_INDEX_OF(addr)         (((addr) >> 39) & 0x1FF)
#define PDPT_INDEX_OF(addr)         (((addr) >> 30) & 0x1FF)
#define PAGE_DIR_INDEX_OF(addr)     (((addr) >> 21) & 0x1FF)
#define PAGE_TABLE_INDEX_OF(addr)   (((addr) >> 12) & 0x1FF)
#define PAGE_FLAG_PRESENT           (1 << 0)
#define PAGE_FLAG_WRITABLE          (1 << 1)
#define PAGE_FLAG_USER              (1 << 2)
#define PAGE_FLAG_WRITETHROUGH      (1 << 3)
#define PAGE_FLAG_NOCACHE           (1 << 4)
#define IS_PAGE_ALIGNED(addr)       (addr % ARCH_PAGE_SIZE == 0)

typedef struct pml4_entry
{
    bool present : 1;       // Must be 1
    bool writable : 1;      // Page is readonly if set to 0, also called Read/Write bit
    bool usr : 1;           // Everyone could access this page if it's not 0, or only supervisor allowed.
    bool write_through : 1; // Write-Through cache is enabled when this bit is set
    bool cache : 1;         // Disable cache if it's set to 1
    bool access : 1;        // Is this page entry has been used.
    int ign_6_11 : 6;       // Ignored
    uint64_t addr : 36;     // Physical Address
    int ign_48_62 : 15;     // Ignored
    bool disable_exec: 1;
} __attribute__((packed)) pml4_entry_t;

typedef struct pml3_entry
{
    bool present : 1;       // Must be 1
    bool writable : 1;      // Page is readonly if set to 0, also called Read/Write bit
    bool usr : 1;           // Everyone could access this page if it's not 0, or only supervisor allowed.
    bool write_through : 1; // Write-Through cache is enabled when this bit is set
    bool cache : 1;         // Disable cache if it's set to 1
    bool access : 1;        // Is this page entry has been used.
    int ign_6 : 1;
    int size : 1;
    int ign_8_11 : 4;
    uint64_t addr : 36;
    int ign_48_62 : 15;
    bool disable_exec : 1;
} __attribute__((packed)) pdpt_entry_t;

typedef struct pml2_entry
{
    bool present : 1;       // Must be 1
    bool writable : 1;      // Page is readonly if set to 0, also called Read/Write bit
    bool usr : 1;           // Everyone could access this page if it's not 0, or only supervisor allowed.
    bool write_through : 1; // Write-Through cache is enabled when this bit is set
    bool cache : 1;         // Disable cache if it's set to 1
    bool access : 1;        // Is this page entry has been used.
    int ign_6 : 1;
    int size : 1;
    int ign_8_11 : 4;
    uint64_t addr : 36;
    int ign_48_62 : 15;
    bool disable_exec : 1;
} __attribute__((packed)) pd_entry_t;

typedef struct page_entry
{
    bool present : 1;      // Must be 1
    bool writable : 1;     // Page is readonly if set to 0, also called Read/Write bit
    bool usr : 1;          // Everyone could access this page if it's not 0, or only supervisor allowed.
    bool writeThrough : 1; // Write-Through cache is enabled when this bit is set
    bool cache : 1;        // Disable cache if it's set to 1
    bool access : 1;       // Is this page entry has been used.
    int dirty : 1;
    int mem_type : 1;
    int global : 1;
    int ign_9_11 : 3;
    uint64_t addr : 36;
    int ign_48_57 : 10;
    uint8_t key : 5;
    bool disable_exec : 1;
} __attribute__((packed)) page_t;

typedef struct pml4
{
    pml4_entry_t entries[PDPTS_PER_PML4];
} pml4_t; /* 512GiB -> 256TiB */

typedef struct pdpt
{
    pdpt_entry_t entries[DIRS_PER_PDPT];
} pdpt_t; /* 1GiB -> 512GiB */

typedef struct page_dir
{
    pd_entry_t entries[TABLES_PER_DIR];
} page_dir_t; /* 2MiB -> 1GiB */

typedef struct page_table
{
    page_t entries[PAGES_PER_TABLE];
} page_table_t; /* 4KiB -> 2MiB */

typedef uint64_t page_flags_t;

extern pml4_t* asmw_get_pagemap(void);
static inline void asmi_load_paging(uintptr_t addr) { asm("mov %%rax, %%cr3" ::"a"((uint64_t)addr)); }
static inline void asmi_invlpg(uintptr_t addr) { asm volatile("invlpg (%0)"::"r"(addr)); }
static inline size_t pml4_indexof(uintptr_t addr) { return (addr >> 39) & 0x1ff; }
static inline size_t pdpt_indexof(uintptr_t addr) { return (addr >> 30) & 0x1ff; }
static inline size_t pd_indexof(uintptr_t addr) { return (addr >> 21) & 0x1ff; }
static inline size_t page_indexof(uintptr_t addr) { return (addr >> 12) & 0x1ff; }
/**
 * @brief Initialize kernel page map and virtual memory management
 */
void vmm_init(void);
/**
 * @brief 
 * 
 * @param map 
 * @param addr 
 * @return true If the page is present (in the memory)
 * @return false If the page does not exist or be swapped into disks.
 */
bool vmm_page_present(
    pml4_t *map, 
    uint64_t addr
);
void map_virtual_address(
    pml4_t *map,
    uint64_t phys,
    uint64_t virt,
    size_t amount,
    page_flags_t flags
);
uintptr_t vmm_alloc_pages(
    pml4_t *map,
    size_t amount
);
void vmm_free_pages(
    pml4_t *map,
    uint64_t virt,
    size_t amount
);
void mark_pages_used(
    pml4_t *map,
    uint64_t virt, 
    size_t amount
);
void mark_pages_free(
    pml4_t *map,
    uint64_t virt,
    size_t amount
);
uintptr_t virt_to_phys(
    pml4_t *pml4, 
    uintptr_t addr
);
void switch_page_tables(pml4_t *map);
pml4_t* current_page_map();
pml4_t* get_kernel_pages();
