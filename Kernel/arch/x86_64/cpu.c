#include <x86_64/cpu.h>
#include <x86_64/apic.h>

int GetCpuNum()
{
    if(m_localApic)
        return m_localApic[LOCAL_APIC_ID / 4] >> 24;
    else
        return 0;
}

static void SetCPULocal(cpu_info_t *cpu)
{
    cpu->self = cpu;
    asm volatile("wrmsr" ::"a"((uintptr_t)cpu & 0xFFFFFFFF) /*Value low*/,
                 "d"(((uintptr_t)cpu >> 32) & 0xFFFFFFFF) /*Value high*/, "c"(0xC0000102) /*Set Kernel GS Base*/);
    asm volatile("wrmsr" ::"a"((uintptr_t)cpu & 0xFFFFFFFF) /*Value low*/,
                 "d"(((uintptr_t)cpu >> 32) & 0xFFFFFFFF) /*Value high*/, "c"(0xC0000101) /*Set Kernel GS Base*/);
}

static cpu_info_t *GetCPULocal()
{
    cpu_info_t *cpu;
    DisableInterrupts();
    asm volatile("swapgs; movq %%gs:0, %0; swapgs;"
                 : "=r"(cpu)); // CPU info is 16-byte aligned as per liballoc alignment
    EnableInterrupts();
    return cpu
}