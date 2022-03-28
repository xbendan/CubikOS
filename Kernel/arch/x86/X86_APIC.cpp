#include <X86_APIC.h>

namespace Arch::x86
{
    static volatile uint32_t* apicLocalPtr = nullptr;

    void ApicLocalWriteData(uint32_t r, uint32_t data)
    {

    }

    uint32_t ApicLocalReadData(uint32_t r)
    {
        return *((volatile uint32_t*)(apicLocalPtr + r));
    }

    void LoadApic()
    {
        
    }
}