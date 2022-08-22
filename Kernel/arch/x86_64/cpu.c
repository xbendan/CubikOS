#include <x86_64/cpu.h>
#include <x86_64/apic.h>
#include <x86_64/interrupts.h>

int GetCpuNum()
{
    if(m_localApic)
        return m_localApic[LOCAL_APIC_ID / 4] >> 24;
    else
        return 0;
}

cpuid_info_t CPUID()
{
    cpuid_info_t info;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;

    asm volatile("cpuid" : "=b"(ebx), "=d"(edx), "=c"(ecx) : "a"(0)); // Get vendor string
    for (int i = 0; i < 4; i++)
        info.vendor_str[i] = ebx >> (i * 8) & 0xFF; // Copy string to buffer
    for (int i = 0; i < 4; i++)
        info.vendor_str[i + 4] = edx >> (i * 8) & 0xFF;
    for (int i = 0; i < 4; i++)
        info.vendor_str[i + 8] = ecx >> (i * 8) & 0xFF;

    asm volatile("cpuid" : "=d"(edx), "=c"(ecx) : "a"(1)); // Get features
    info.ecx = ecx;
    info.edx = edx;
    return info;
}

void SetCPULocal(cpu_info_t *cpu)
{
    cpu->self = cpu;
    asm volatile("wrmsr" ::"a"((uintptr_t)cpu & 0xFFFFFFFF) /*Value low*/,
                 "d"(((uintptr_t)cpu >> 32) & 0xFFFFFFFF) /*Value high*/, "c"(0xC0000102) /*Set Kernel GS Base*/);
    asm volatile("wrmsr" ::"a"((uintptr_t)cpu & 0xFFFFFFFF) /*Value low*/,
                 "d"(((uintptr_t)cpu >> 32) & 0xFFFFFFFF) /*Value high*/, "c"(0xC0000101) /*Set Kernel GS Base*/);
}

cpu_info_t *GetCPULocal()
{
    cpu_info_t *cpu;
    DisableInterrupts();
    asm volatile("swapgs; movq %%gs:0, %0; swapgs;"
                 : "=r"(cpu)); // CPU info is 16-byte aligned as per liballoc alignment
    EnableInterrupts();
    return cpu;
}