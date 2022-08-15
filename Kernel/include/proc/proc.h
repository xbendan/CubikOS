#pragma once

#include <macros.h>

;
typedef uint16_t pid_t;
typedef uint32_t tid_t;

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

typedef struct Process
{
    char *name;
    char *publisher;
    char *filename;
    pid_t pid;
    uint8_t type;
    union
    {
        uint32_t size;
        struct
        {
            bool focus: 1;
            uint32_t reserved: 31;
        } __attribute__((packed));
    };
    uint16_t threads;
    uint16_t handles;
    uint16_t cores;
    struct
    {
        uint64_t allocated;
        uint32_t cached;
        uint32_t pages;
        uint32_t present_pages;
        uint32_t swapped_pages;
    };

    uintptr_t entryPoint;
    uintptr_t heap;

    #ifdef ARCH_X86_64
    void *page_map;
    uint64_t** vmmap;
    #elif ARCH_AARCH64

    #elif ARCH_RISCV

    #endif
} proc_t;

typedef struct Thread
{
    tid_t tid;
    proc_t* proc;
} thread_t;