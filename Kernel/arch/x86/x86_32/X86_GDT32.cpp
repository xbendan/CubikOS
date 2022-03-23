#include <X86_GDT32.h>

/**
 * @brief code rewrited from skiftOS.
 * 
 */
namespace Arch::x86_32
{
    static TaskStackSegment tss = {
        .prev_tss = 0,
        .esp0 = 0,
        .ss0 = 0x10,
        .esp1 = 0,
        .ss1 = 0,
        .esp2 = 0,
        .ss2 = 0,
        .cr3 = 0,
        .eip = 0,
        .eflags = 0x0202,
        .eax = 0,
        .ecx = 0,
        .edx = 0,
        .ebx = 0,
        .esp = 0,
        .ebp = 0,
        .esi = 0,
        .edi = 0,
        .es = 0,
        .cs = 0,
        .ss = 0,
        .ds = 0,
        .fs = 0,
        .gs = 0,
        .ldt = 0,
        .trap = 0,
        .iomap_base = 0,
    };
    static GDT32Entry gdt[GDT_ENTRY_COUNT];
    static GDT32Pack gdtr = {
        .limit = sizeof(GDT32Entry) * GDT_ENTRY_COUNT,
        .base = (uint32_t) &gdt[0]
    };

    void SetupGDT()
    {
        // Null descriptor
        gdt[0] = GDT32Entry(0, 0, 0, 0);
        // Code
        gdt[1] = GDT32Entry(
            0,
            0xFFFFFFFF,
            GDT_PRESENT | GDT_READWRITE | GDT_EXECUTABLE, // 10011010
            GDT_FLAGS
        );
        // Data
        gdt[2] = GDT32Entry(
            0,
            0xFFFFFFFF,
            GDT_PRESENT | GDT_READWRITE, // 10010010
            GDT_FLAGS
        );
        // UserCode
        gdt[3] = GDT32Entry(
            0,
            0xFFFFFFFF,
            GDT_PRESENT | GDT_READWRITE | GDT_USER | GDT_EXECUTABLE, // 11111010
            GDT_FLAGS
        );
        // UserData
        gdt[4] = GDT32Entry(
            0,
            0xFFFFFFFF,
            GDT_PRESENT | GDT_READWRITE | GDT_USER, // 11110010
            GDT_FLAGS
        );
        // TSS
        gdt[5] = GDT32Entry(
            (uint32_t) &tss,
            ((uint32_t) &tss) + sizeof(TaskStackSegment),
            GDT_TSS_PRESENT | GDT_ACCESSED | GDT_EXECUTABLE | GDT_USER, 
            TSS_FLAGS
        );

        __flushGDT((uint32_t) &gdtr);
    }
}