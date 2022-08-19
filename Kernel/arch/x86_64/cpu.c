#include <x86_64/cpu.h>
#include <x86_64/apic.h>

int GetCpuNum()
{
    if(m_localApic)
        return m_localApic[LOCAL_APIC_ID / 4] >> 24;
    else
        return 0;
}