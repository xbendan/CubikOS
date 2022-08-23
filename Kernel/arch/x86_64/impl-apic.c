#include <x86_64/paging.h>
#include <x86_64/apic.h>
#include <x86_64/pic.h>
#include <panic.h>

volatile uint32_t* apicLocalPtr = NULL;

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
    apicLocalPtr = (uint32_t *) VM_GetIOMapping(LAPIC_ReadBase());
    LAPIC_WriteData(
        LOCAL_APIC_SIVR,
        LAPIC_ReadData(LOCAL_APIC_SIVR) | 0x100);
    asm("hlt");
}

uintptr_t ioApicBase;
uintptr_t ioApicVirtBase;
volatile uint32_t *ioApicRegisterSelect;
volatile uint32_t *ioApicWindow;
uint32_t ioApicInterrupts;
uint32_t ioApicId;

void IOAPIC_Initialize()
{
    if(!ioApicBase)
    {
        CallPanic("[APIC] Attempt to initialize I/O APIC with out base");
        return;
    }

    ioApicVirtBase = VM_GetIOMapping(ioApicBase);
    ioApicRegisterSelect = (uint32_t *)(ioApicVirtBase + IO_APIC_REGSEL);
    ioApicWindow = (uint32_t *)(ioApicVirtBase + IO_APIC_WIN);
    ioApicInterrupts = IOAPIC_ReadData32(IO_APIC_REGISTER_VER) >> 16;
    ioApicId = IOAPIC_ReadData32(IO_APIC_REGISTER_ID) >> 24;

    //for(int i = 0; i < ACPI)
}

void IOAPIC_SetBase(uintptr_t newBase)
{
    ioApicBase = newBase;
}

void IOAPIC_MapLegacyIRQ(uint8_t irq);