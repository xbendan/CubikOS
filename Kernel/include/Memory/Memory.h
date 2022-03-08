#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#ifndef MEM_PAGE_SIZE
    #define MEM_PAGE_SIZE 4096
#endif

namespace Memory {
    typedef struct MemoryInfo {
        size_t maxSize;
    }
    
    typedef struct MemoryPageDir {
        bool present; // Must be 1
        bool writable; // Page is readonly if set to 0, also called Read/Write bit
        bool uaccess; // Everyone could access this page if it's not 0, or only supervisor allowed.
        bool writeThrough; // Write-Through cache is enabled when this bit is set
        bool cache; // Disable cache if it's set to 1
        bool access; // Is this page entry has been used.
        bool dirty; // Page has been written sth in
        
    } __attribute__((packed)) memory_page_dir_t;
}