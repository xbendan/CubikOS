#include <stdint.h>
#include <stddef.h>
#include <Misc.h>
#define MEM_PAGE_SIZE       4096
#define PAGE_DIRECTORY_INDEX(vaddr) ((vaddr) >> 22)
#define PAGE_TABLE_INDEX(vaddr) (((vaddr) >> 12) & 0x03ff)
#define PAGE_TABLE_ENTRY_COUNT 1024
#define PAGE_DIRECTORY_ENTRY_COUNT 1024
#define SIZE_TO_PAGE(size) (size / MEM_PAGE_SIZE)
#define IS_PAGE_ALIGNED(addr) (addr % MEM_PAGE_SIZE == 0)

namespace Paging {
    union PageTableEntry
    {
        struct
        {
            bool present : 1;
            bool write : 1;
            bool user : 1;
            bool writeThrough : 1;
            bool cache : 1;
            bool uaccess : 1;
            bool dirty : 1;
            bool pat : 1;
            uint32_t ign : 4;
            uint32_t frameNum : 20;
        } __attribute__((packed));
        uint32_t as_uint;
    } __attribute__((packed));

    struct PageTable
    {
        PageTableEntry entries[PAGE_TABLE_ENTRY_COUNT];
    } __attribute__((packed));

    union PageDirectoryEntry
    {
        struct 
        {
            bool present : 1;
            bool write : 1;
            bool user : 1;
            bool writeThrough : 1;
            bool cache : 1;
            bool uaccess : 1;
            bool ign1 : 1;
            bool largePage : 1;
            uint32_t ign2 : 4;
            uint32_t frameNum : 20;
        } __attribute__((packed));
        uint32_t as_uint;
    } __attribute__((packed));

    struct PageDirectory : public Addrspace
    {
        PageDirectoryEntry entries[PAGE_DIRECTORY_ENTRY_COUNT];
    } __attribute__((packed));

    extern "C" void __loadPaging(uintptr_t addr);
    extern "C" void __enablePaging();
    extern "C" void __disablePaging();

    void Initialize();
    void Enable();
    void Disable();
}