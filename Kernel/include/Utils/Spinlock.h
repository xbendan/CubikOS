typedef struct Spinlocker
{
    volatile uint32_t val;
} lock_t;

namespace Utils::Spinlock
{
    static inline void LockInit(lock_t* lock)
    {
        lock->val = 0;
    }

    static inline void AcquireLock(lock_t* lock)
    {
        int lock_val = 1; // From DPDK
        __asm__ volatile(
                "1:\n"
                "xchg %[value], %[lv]\n"
                "test %[lv], %[lv]\n"
                "jz 3f\n"
                "2:\n"
                "pause\n"
                "cmpl $0, %[value]\n"
                "jnz 2b\n"
                "jmp 1b\n"
                "3:\n"
                : [value] "=m" (lock->val), [lv] "=q" (lock_val)
                : "[lv]" (lock_val)
                : "memory");
    }

    static inline void ReleaseLock(lock_t* lock)
    {
        
    }
}