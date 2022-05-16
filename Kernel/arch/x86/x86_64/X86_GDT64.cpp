#include <X86_GDT64.h>

namespace Arch::x86_64
{
    static Tss64 tss = {
        .reserved = 0,
        .rsp = {},
        .reserved0 = 0,
        .ist = {},
        .reserved1 = 0,
        .reserved2 = 0,
        .reserved3 = 0,
        .iopb_offset = 0,
    };
    static gdt_pack_t gdt = {};
    static gdt_ptr_t gdtr = {
        .size = sizeof(GDT64Pack) - 1,
        .base = (uint64_t) &gdt
    };

    void SetupGDT()
    {
        gdt.entries[0] = GDT64Entry(0, 0, 0, 0);
        gdt.entries[1] = GDT64Entry(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_EXECUTABLE, GDT_LM_GRANULARITY);
        gdt.entries[2] = GDT64Entry(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE, 0);
        gdt.entries[3] = GDT64Entry(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_EXECUTABLE | GDT_USER, GDT_LM_GRANULARITY);
        gdt.entries[4] = GDT64Entry(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_USER, 0);

        gdt.tss = GDT64TssEntry((uintptr_t) &tss);

        asmw_flush_gdt((uint64_t)&gdtr);
    }
}