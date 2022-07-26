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
static struct gdt_pack gdt = {};
static struct gdt_ptr gdtr = {
    .size = sizeof(gdt_pack) - 1,
    .base = (uint64_t) &gdt
};

void lgdt()
{
    gdt.entries[0] = gdt_entry(0, 0, 0, 0);
    gdt.entries[1] = gdt_entry(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_EXECUTABLE, GDT_LM_GRANULARITY);
    gdt.entries[2] = gdt_entry(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE, 0);
    gdt.entries[3] = gdt_entry(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_EXECUTABLE | GDT_USER, GDT_LM_GRANULARITY);
    gdt.entries[4] = gdt_entry(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_USER, 0);
    gdt.tss = gdt_tss_entry((uintptr_t) &tss);

    asmw_flush_gdt((uint64_t) &gdtr);
}