#include <x86_64/smp.h>
#include <x86_64/cpu.h>
#include <x86_64/acpi.h>
#include <mm/malloc.h>

extern struct GlobalDescTablePointer g_Gdtr;
extern struct InterruptDescTablePointer g_Idtr;

cpu_info_t *cpus[MAX_CPU_AMOUNT];

void SMP_Initialize()
{
    cpus[0] = (cpu_info_t *) KernelAllocateObject(sizeof(cpu_info_t));

    *cpus[0] = (cpu_info_t)
    {
        .id = 0,
        .gdt = (void *) g_Gdtr.base,
        .gdtPtr = g_Gdtr,
        .idtPtr = g_Idtr,
        .currentThread = NULL,
        .idleThread = NULL
    };

    SetCPULocal(cpus[0]);
}