#pragma once

#include <fs/vfs.h>
#include <proc/activity.h>
#include <utils/list.h>
#include <utils/spinlock.h>
#include <macros.h>

#ifdef ARCH_X86_64
    #include <x86_64/cpu.h>
#elif ARCH_AARCH64

#elif ARCH_RISCV

#endif

;

typedef uint32_t tid_t;
typedef uint16_t pid_t;

enum TaskPriority
{
    PriorityIdle = -1,
    PriorityLowest = 0,
    PriorityLow = 1,
    PriorityNormal = 2,
    PriorityHigh = 3,
    PriorityHighest = 4,
    PriorityRealTime = 5
};

enum TaskState
{
    TaskStateRunning = 0,
    TaskStateIdle = 1,
};

enum TaskType
{
    TaskTypeSystemProcess = 0,
    TaskTypeSystemDriver = 1,
    TaskTypeApplication = 2,
    TaskTypeService = 3,
    TaskTypeBackground = 4
};

enum ThreadState
{
    ThreadStateAbolished = 0,
    ThreadStateRunning = 1,
    ThreadStateBlocked = 2,
    ThreadState
};

struct Process
{
    char *name;         /* Name of the process */
    char *publisher;    /* Name of the publisher */
    char *filename;     /* Executable file name of this process */
    char *package;      /* Package Name */
    pid_t pid;          /* Process Id, 0~255 are reserved for kernel process */
    uint8_t type;       /* Current process type */
    struct File *file;
    struct Activity *activity;   /* Pointer to the Activity */
    union
    { // Flags
        uint32_t size;
        struct
        {
            bool focus: 1; 
            uint32_t reserved: 31;
        } __attribute__((packed));
    };
    uint16_t threads;   /* Amount of threads kept by this process*/
    uint16_t handles;   /* Register handles amount */
    //uint16_t cores;     /* CPU Cores currently running on */
    struct
    { // Memory
        uint64_t allocated;     /* Memory allocated for this process */
        uint32_t cached;        /* Memory allocated and cached */
        uint32_t pages;         /* Amount of 4K pages */
        uint32_t present_pages; /* Pages that is present now */
        uint32_t swapped_pages; /* Pages that has been swapped into disks */
    };

    uintptr_t entry_point;
    uintptr_t heap;

    /* Architecture Fields */
    #ifdef ARCH_X86_64
    void *page_map;
    uint64_t** vmmap;
    #elif ARCH_AARCH64

    #elif ARCH_RISCV

    #endif
};

struct Thread
{
    tid_t tid;              /* Thread ID, not duplicated in same progress */
    struct Process *process;        /* Parent process, indicates the owner of this thread */
    spinlock_t lock;        /* Thread lock */
    spinlock_t stateLock;   /* Thread state lock */

    struct RegisterContext registers;  
    struct RegisterContext last_syscall;
    
    uintptr_t stackBase;
    uintptr_t stackSize;

    uint8_t priority;       /* The priority when scheduling */
    uint8_t state;          /* Thread state */
};