#pragma once

typedef volatile uint32_t spinlock_t;

/*
typedef struct Spinlocker
{
    volatile uint32_t val;
} lock_t;
*/

namespace Utils::Spinlock
{
    static inline void LockInit(spinlock_t* lock)
    {
        lock = 0;
    }

    static inline void AcquireLock(spinlock_t* lock)
    {
        int lockVal = 1; // From DPDK
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
                : [value] "=m" (*lock), [lv] "=q" (lockVal)
                : "[lv]" (lockVal)
                : "memory");
    }

    static inline void ReleaseLock(spinlock_t* lock)
    {
        int unlockVal = 0;
        __asm__ volatile(
                "xchg %[value], %[ulv]\n"
                : [value] "=m" (*lock), [ulv] "=q" (unlockVal)
                : "[ulv]" (unlockVal)
                : "memory");
    }
}