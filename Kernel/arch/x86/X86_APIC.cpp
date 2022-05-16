#include <X86_APIC.h>
#include <X86_PIC.h>

namespace Arch::x86
{
    static volatile uint32_t* apicLocalPtr = nullptr;

    void ApicLocalWriteBase(uint64_t val)
    {
        uint64_t low = val & 0xFFFFFFFF;
        uint64_t high = val >> 32;
        asm("wrmsr" ::"a"(low), "d"(high), "c"(0x1B));
    }

    uint64_t ApicLocalReadBase()
    {
        uint64_t low;
        uint64_t high;
        asm("rdmsr" : "=a"(low), "=d"(high) : "c"(0x1B));

        return (high << 32) | low;
    }

    void ApicLocalWriteData(uint32_t reg, uint32_t data)
    {
        *((volatile uint32_t*)(apicLocalPtr + reg)) = data;
    }

    uint32_t ApicLocalReadData(uint32_t reg)
    {
        return *((volatile uint32_t*)(apicLocalPtr + reg));
    }

    void ApicIoWriteData_32(uint32_t reg, uint32_t data)
    {

    }

    uint32_t ApicIoReadData_32(uint32_t reg)
    {

    }

    void ApicIoWriteData_64(uint32_t reg, uint64_t data)
    {

    }

    uint64_t ApicIoReadData_64(uint32_t reg)
    {

    }

    void ApicTimerStart()
    {
        ApicLocalWriteData(LOCAL_APIC_TIMER_DIVIDE, 0x3);
    }

    void ApicLocalInitialize()
    {
        UnloadPic();
        apicLocalPtr = (uint32_t*)ApicLocalReadBase();
        ApicLocalWriteData(
            LOCAL_APIC_SIVR, 
            ApicLocalReadData(LOCAL_APIC_SIVR) | 0x100
        );
    }
}