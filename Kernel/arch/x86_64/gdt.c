#include <x86_64/gdt.h>
#include <x86_64/cpu.h>

static struct task_state_segment tss = {
    .ign_0 = 0,
    .rsp = {},
    .ign_1 = 0,
    .ist = {},
    .ign_2 = 0,
    .ign_3 = 0,
    .ign_4 = 0,
    .iopb_offset = 0,
};
static struct gdt_pack gdt = {
    {
		{0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00},
		{0xFFFF, 0x0000, 0x00, 0x9A, (1 << 5) | (1 << 7) | 0x0F, 0x00},
		{0xFFFF, 0x0000, 0x00, 0x92, (1 << 5) | (1 << 7) | 0x0F, 0x00},
		{0xFFFF, 0x0000, 0x00, 0xFA, (1 << 5) | (1 << 7) | 0x0F, 0x00},
		{0xFFFF, 0x0000, 0x00, 0xF2, (1 << 5) | (1 << 7) | 0x0F, 0x00}
	},
    {
    }
};
static struct gdt_ptr gdtr = {
    .size = sizeof(struct gdt_pack) - 1,
    .base = (uint64_t) &gdt,
};

void lgdt()
{
    /*
    gdt.entries[0] = (struct gdt_entry){
        .limit_low = 0,
        .base_low = 0,
        .base_medium = 0,
        .flags = 0,
        .granularity = 0,
        .base_high = 0
    };
    gdt.entries[1] = (struct gdt_entry){
        .limit_low = 0,
        .base_low = 0,
        .base_medium = 0,
        .flags = (GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_EXECUTABLE),
        .granularity = GDT_LM_GRANULARITY,
        .base_high = 0
    };
    gdt.entries[2] = (struct gdt_entry){
        .limit_low = 0,
        .base_low = 0,
        .base_medium = 0,
        .flags = (GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE),
        .granularity = 0,
        .base_high = 0
    };
    gdt.entries[3] = (struct gdt_entry){
        .limit_low = 0,
        .base_low = 0,
        .base_medium = 0,
        .flags = (GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_EXECUTABLE | GDT_USER),
        .granularity = GDT_LM_GRANULARITY,
        .base_high = 0
    };
    gdt.entries[4] = (struct gdt_entry){
        .limit_low = 0,
        .base_low = 0,
        .base_medium = 0,
        .flags = (GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_USER),
        .granularity = 0,
        .base_high = 0
    };
    */
    uintptr_t tss_addr = (uintptr_t) &tss;
    gdt.tss = (struct gdt_tss_entry){
        .len = sizeof(struct task_state_segment),
        .base_low = tss_addr & 0xffff,
        .base_medium = (tss_addr >> 16) & 0xff,
        .flags_a = 0b10001001,
        .flags_b = 0b0,
        .base_high = (tss_addr >> 24) & 0xff,
        .base_upper = (tss_addr >> 32),
        .ign = 0
    };

    asmw_flush_gdt((uint64_t) &gdtr);
}