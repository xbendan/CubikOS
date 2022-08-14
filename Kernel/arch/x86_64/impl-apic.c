#include <x86_64/apic.h>
#include <x86_64/pic.h>

static volatile uint32_t* apicLocalPtr = nullptr;

void lapic_writebase(uint64_t val)
{
    uint64_t low = val & 0xFFFFFFFF;
    uint64_t high = val >> 32;
    asm("wrmsr" ::"a"(low), "d"(high), "c"(0x1B));
}

uint64_t lapic_readbase()
{
    uint64_t low;
    uint64_t high;
    asm("rdmsr" : "=a"(low), "=d"(high) : "c"(0x1B));

    return (high << 32) | low;
}

void lapic_writedata(uint32_t reg, uint32_t data)
{
    *((volatile uint32_t*)(apicLocalPtr + reg)) = data;
}

uint32_t lapic_readdata(uint32_t reg)
{
    return *((volatile uint32_t*)(apicLocalPtr + reg));
}

void ioapic_writedata32(uint32_t reg, uint32_t data)
{
}

uint32_t ioapic_readdata32(uint32_t reg)
{
}

void ioapic_writedata64(uint32_t reg, uint64_t data)
{
}

uint64_t ioapic_readdata64(uint32_t reg)
{
}

void apic_start_timer()
{
    lapic_writedata(LOCAL_APIC_TIMER_DIVIDE, 0x3);
}

void lapic_initialize()
{
    pic_unload();
    apicLocalPtr = (uint32_t *)lapic_readbase();
    lapic_writedata(
        LOCAL_APIC_SIVR,
        lapic_readdata(LOCAL_APIC_SIVR) | 0x100);
}