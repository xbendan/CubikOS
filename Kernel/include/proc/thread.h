#pragma once

#include <proc/proc.h>

typedef struct Thread
{
    tid_t tid;
    proc_t* proc;
    
    struct
    {
        uint32_t esp0;
        uint16_t ss0;
        uint32_t stack;
    } __attribute__((packed));
    
    uintptr_t stack;
    uintptr_t stackBase;
    uintptr_t stackSize;
} thread_t;