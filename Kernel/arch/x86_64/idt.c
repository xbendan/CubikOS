#include <x86_64/idt.h>

extern uintptr_t isr_tables[];
static struct idt_entry idt_entry_list[IDT_ENTRY_COUNT];
static struct idt_pack idtr = {
    .size = sizeof(struct idt_entry) * IDT_ENTRY_COUNT,
    .base = (uint64_t) &idt_entry_list[0]
};

void lidt()
{
    for (uint8_t num = 0; num < 48; num++)
        idt_entry_list[num] = (struct idt_entry){
            .base_low = (isr_tables[num] & 0xffff),
            .selector = 0x08,
            .ist = 0x0,
            .flags = IDT_FLAGS_INTGATE,
            .base_med = ((isr_tables[num] >> 16) & 0xffff),
            .base_high = ((isr_tables[num] >> 32) & 0xffffffff)
        };
        

    idt_entry_list[127] = (struct idt_entry){
        .base_low = (isr_tables[32] & 0xffff),
        .selector = 0x08,
        .ist = 0x0,
        .flags = IDT_FLAGS_INTGATE,
        .base_med = ((isr_tables[32] >> 16) & 0xffff),
        .base_high = ((isr_tables[32] >> 32) & 0xffffffff)
    };
    idt_entry_list[128] = (struct idt_entry){
        .base_low = (isr_tables[33] & 0xffff),
        .selector = 0x08,
        .ist = 0x0,
        .flags = IDT_FLAGS_INTGATE | IDT_FLAGS_USER,
        .base_med = ((isr_tables[33] >> 16) & 0xffff),
        .base_high = ((isr_tables[33] >> 32) & 0xffffffff)
    };

    asmw_flush_idt((uint64_t) &idtr);
}