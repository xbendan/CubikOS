#include <X86_IDT32.h>

extern "C" uintptr_t isrTable[];
//__attribute__((aligned(0x10)))
    idt_entry_t  idt[IDT_ENTRY_COUNT];
static idt_ptr_t    idtr = {
    .limit = sizeof(IDT32Entry) * IDT_ENTRY_COUNT,
    .base = (uint32_t)&idt[0]
};

namespace Arch::x86_32 {

    void SetupIDT()
    {
        for (uint8_t num = 0; num < 48; num++)
        {
            idt[num] = IDT32Entry(isrTable[num], 0x08, IDT_FLAGS_INTGATE);
        }
        idt[3] = IDT32Entry(isrTable[3], 0x08, IDT_FLAGS_TRAPGATE);
        idt[4] = IDT32Entry(isrTable[3], 0x08, IDT_FLAGS_TRAPGATE);

        idt[127] = IDT32Entry(isrTable[48], 0, IDT_FLAGS_INTGATE);
        idt[128] = IDT32Entry(isrTable[49], 0, IDT_FLAGS_INTGATE | IDT_FLAGS_USER);

        __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    }

    void RegInterrupt(uint8_t intr, isr_t func, void* data)
    {

    }

    void SetIDTEntry(uint8_t vec, uint64_t base, uint16_t selector, uint8_t flags);
}