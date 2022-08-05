typedef volatile uint16_t spinlock_t;

static inline void spinlock_init(spinlock_t* lock)
{
    lock = 0;
}

static inline void acquire_spinlock(spinlock_t* lock)
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

static inline void release_spinlock(spinlock_t* lock)
{
    int unlockVal = 0;
    __asm__ volatile(
            "xchg %[value], %[ulv]\n"
            : [value] "=m" (*lock), [ulv] "=q" (unlockVal)
            : "[ulv]" (unlockVal)
            : "memory");
}