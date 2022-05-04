#include <X86_APIC.h>
#include <X86_PIC.h>

namespace Arch::x86
{
    static volatile uint32_t* apicLocalPtr = nullptr;

    void ApicLocalWriteData_32(uint32_t reg, uint32_t data)
    {

    }

    uint32_t ApicLocalReadData_32(uint32_t reg)
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
        ApicLocalWriteData_32(LOCAL_APIC_TIMER_DIVIDE, 0x3);
    }

    void LoadApic()
    {
        UnloadPic();
    }
}