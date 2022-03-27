#include <X86_IDT32.h>
#include <X86_INTR32.h>
#include <X86_PIC.h>

extern "C"
{
    void irq0();
    void irq1();
    void irq2();
    void irq3();
    void irq4();
    void irq5();
    void irq6();
    void irq7();
    void irq8();
    void irq9();
    void irq10();
    void irq11();
    void irq12();
    void irq13();
    void irq14();
    void irq15();
}

namespace Arch::x86_32
{
    extern "C" uintptr_t isrTable[];
           idt_entry_t  idt[IDT_ENTRY_COUNT];
    static idt_ptr_t    idtr = {
        .limit = sizeof(IDT32Entry) * IDT_ENTRY_COUNT - 1,
        .base = (uint32_t)&idt[0]
    };

    void SetupIDT()
    {
        for (uint8_t num = 0; num < 32; num++)
        {
            idt[num] = IDT32Entry(isrTable[num], 0x08, IDT_FLAGS_INTGATE);
        }
        idt[3] = IDT32Entry(isrTable[3], 0x08, IDT_FLAGS_TRAPGATE);
        idt[4] = IDT32Entry(isrTable[4], 0x08, IDT_FLAGS_TRAPGATE);

        idt[32] = IDT32Entry((uint32_t)irq0, 0x08, IDT_FLAGS_INTGATE);
        idt[33] = IDT32Entry((uint32_t)irq1, 0x08, IDT_FLAGS_INTGATE);
        idt[34] = IDT32Entry((uint32_t)irq2, 0x08, IDT_FLAGS_INTGATE);
        idt[35] = IDT32Entry((uint32_t)irq3, 0x08, IDT_FLAGS_INTGATE);
        idt[36] = IDT32Entry((uint32_t)irq4, 0x08, IDT_FLAGS_INTGATE);
        idt[37] = IDT32Entry((uint32_t)irq5, 0x08, IDT_FLAGS_INTGATE);
        idt[38] = IDT32Entry((uint32_t)irq6, 0x08, IDT_FLAGS_INTGATE);
        idt[39] = IDT32Entry((uint32_t)irq7, 0x08, IDT_FLAGS_INTGATE);
        idt[40] = IDT32Entry((uint32_t)irq8, 0x08, IDT_FLAGS_INTGATE);
        idt[41] = IDT32Entry((uint32_t)irq9, 0x08, IDT_FLAGS_INTGATE);
        idt[42] = IDT32Entry((uint32_t)irq10, 0x08, IDT_FLAGS_INTGATE);
        idt[43] = IDT32Entry((uint32_t)irq11, 0x08, IDT_FLAGS_INTGATE);
        idt[44] = IDT32Entry((uint32_t)irq12, 0x08, IDT_FLAGS_INTGATE);
        idt[45] = IDT32Entry((uint32_t)irq13, 0x08, IDT_FLAGS_INTGATE);
        idt[46] = IDT32Entry((uint32_t)irq14, 0x08, IDT_FLAGS_INTGATE);
        idt[47] = IDT32Entry((uint32_t)irq15, 0x08, IDT_FLAGS_INTGATE);
        Arch::x86::LoadPIC();

        idt[127] = IDT32Entry(isrTable[48], 0x08, IDT_FLAGS_INTGATE);
        idt[128] = IDT32Entry(isrTable[49], 0x08, IDT_FLAGS_INTGATE | IDT_FLAGS_USER);
        
        __flushIDT((uint32_t) &idtr);
    }

    void RegInterrupt(uint8_t intr, isr_t func, void* data)
    {

    }

    void SetIDTEntry(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags)
    {

    }
}