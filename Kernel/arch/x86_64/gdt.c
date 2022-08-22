#include <x86_64/gdt.h>
#include <x86_64/cpu.h>

static struct TaskStateSegment tss = {
    .ign_0 = 0,
    .rsp = {},
    .ign_1 = 0,
    .ist = {},
    .ign_2 = 0,
    .ign_3 = 0,
    .ign_4 = 0,
    .iopb_offset = 0,
};
static struct GlobalDescTablePackage gdt = {
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
struct GlobalDescTablePointer g_Gdtr = {
    .size = sizeof(struct GlobalDescTablePackage) - 1,
    .base = (uint64_t) &gdt,
};

void LoadGlobalDescTable()
{
    uintptr_t tss_addr = (uintptr_t) &tss;
    gdt.tss = (struct GlobalDescTableTaskEntry){
        .len = sizeof(struct TaskStateSegment),
        .base_low = tss_addr & 0xffff,
        .base_medium = (tss_addr >> 16) & 0xff,
        .flags_a = 0b10001001,
        .flags_b = 0b0,
        .base_high = (tss_addr >> 24) & 0xff,
        .base_upper = (tss_addr >> 32),
        .ign = 0
    };

    asmw_flush_gdt((uint64_t) &g_Gdtr);
}