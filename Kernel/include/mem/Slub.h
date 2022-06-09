#include <utils/Spinlock.h>

namespace Memory::Allocation
{
    typedef struct MemoryCacheCPU
    {
        void** freeList;
        
    } mem_cache_cpu_t;

    typedef struct KernelMemoryCacheNode
    {
        spinlock_t lock;
        uint64_t nr_partial;
        
        
    } kmem_cache_node_t;
}