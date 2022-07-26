extern "C" uintptr_t isr_tables[];
static struct idt_entry idt_entry_list[IDT_ENTRY_COUNT];
static struct idt_ptr idtr = {
    .size = sizeof(idt_entry) * IDT_ENTRY_COUNT,
    .base = (uint64_t) &idt[0]
};

void lidt()
{
    for (uint8_t num = 0; num < 48; num++)
        idt[num] = idt_entry(isr_tables[num], 0x0, IDT_FLAGS_INTGATE);

    idt[127] = idt_entry(isr_tables[32], 0x0, IDT_FLAGS_INTGATE);
    idt[128] = idt_entry(isr_tables[33], 0x0, IDT_FLAGS_INTGATE | IDT_FLAGS_USER);

    asmw_flush_idt((uint64_t) &idtr);
}