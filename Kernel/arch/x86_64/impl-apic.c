#include <x86_64/apic.h>
#include <x86_64/pic.h>

static volatile uint32_t* apicLocalPtr = NULL;
volatile uint32_t *m_localApic = NULL;

void LAPIC_WriteBase(uint64_t val)
{
    uint64_t low = val & 0xFFFFFFFF;
    uint64_t high = val >> 32;
    asm("wrmsr" ::"a"(low), "d"(high), "c"(0x1B));
}

uint64_t LAPIC_ReadBase()
{
    uint64_t low;
    uint64_t high;
    asm("rdmsr" : "=a"(low), "=d"(high) : "c"(0x1B));

    return (high << 32) | low;
}

void LAPIC_WriteData(uint32_t reg, uint32_t data)
{
    *((volatile uint32_t*)(apicLocalPtr + reg)) = data;
}

uint32_t LAPIC_ReadData(uint32_t reg)
{
    return *((volatile uint32_t*)(apicLocalPtr + reg));
}

void IOAPIC_WriteData32(uint32_t reg, uint32_t data)
{
}

uint32_t IOAPIC_ReadData32(uint32_t reg)
{
}

void IOAPIC_WriteData64(uint32_t reg, uint64_t data)
{
}

uint64_t IOAPIC_ReadData64(uint32_t reg)
{
}

void LAPIC_StartTimer()
{
    LAPIC_WriteData(LOCAL_APIC_TIMER_DIVIDE, 0x3);
}

void LAPIC_Initialize()
{
    PIC_Disable();
    apicLocalPtr = (uint32_t *)LAPIC_ReadBase();
    LAPIC_WriteData(
        LOCAL_APIC_SIVR,
        LAPIC_ReadData(LOCAL_APIC_SIVR) | 0x100);
}