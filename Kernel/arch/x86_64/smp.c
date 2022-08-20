#include <x86_64/smp.h>
#include <x86_64/cpu.h>
#include <mm/malloc.h>

extern struct GlobalDescTablePointer gdtr;
extern struct InterruptDescTablePointer idtr;

cpu_info_t *cpus[MAX_CPU_AMOUNT];

void SMP_Initialize()
{
    cpus[0] = KernelAllocateObject(sizeof(cpu_info_t));

    *cpus[0] = (cpu_info_t)
    {
        .id = 0,
        .gdt = (void *) gdtr.base,
        .gdtPtr = gdtr,
        .idtPtr = idtr,
        .currentThread = NULL,
        .idleThread = NULL
    };

    SetCPULocal(cpus[0]);

    
}