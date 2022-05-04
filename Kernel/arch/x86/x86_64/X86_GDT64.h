#pragma once

#include <stdint.h>
#include <stddef.h>

#define GDT_ENTRY_COUNT 5
#define GDT_SEGMENT 0b00010000
#define GDT_PRESENT 0b10000000
#define GDT_USER 0b01100000
#define GDT_EXECUTABLE 0b00001000
#define GDT_READWRITE 0b00000010
#define GDT_LM_GRANULARITY 0b0010
#define GDT_FLAGS 0b1100

namespace Arch::x86_64
{
    typedef struct GDT64Pointer
    {
        uint16_t size;
        uint64_t base;
    } __attribute__((packed)) gdt_ptr_t;

    typedef struct GDT64Entry
    {
        uint16_t limitLow;
        uint16_t baseLow;
        uint8_t baseMedium;
        uint8_t flags;
        uint8_t limitMedium: 4;
        uint8_t granularity: 4;
        uint8_t baseHigh;

        constexpr GDT64Entry() : GDT64Entry(0, 0, 0, 0) {}
        constexpr GDT64Entry(uint32_t base, uint32_t limit, uint8_t granularity, uint8_t flags)
          : limitLow((uint16_t)(limit & 0xFFFF)),
            baseLow((uint16_t)(base & 0xFFFF)),
            baseMedium((uint8_t)((base >> 16) & 0xFF)),
            flags(flags),
            limitMedium((limit >> 16) & 0x0F),
            granularity(granularity),
            baseHigh((uint8_t)((base >> 24) & 0xFF)) {}
        constexpr GDT64Entry(uint8_t flags, uint8_t granularity)
          : GDT64Entry(0, 0, granularity, flags) {}
    } __attribute__((packed)) GDT64Entry;

    struct Tss64
    {
        uint32_t reserved;
        uint64_t rsp[3];
        uint64_t reserved0;
        uint64_t ist[7];
        uint32_t reserved1;
        uint32_t reserved2;
        uint16_t reserved3;
        uint16_t iopb_offset;
    } __attribute__((packed));

    typedef struct GDT64TssEntry
    {
        uint16_t length;
        uint16_t baseLow;
        uint8_t baseMedium;
        uint8_t flags_a;
        uint8_t flags_b;
        uint8_t baseHigh;
        uint32_t baseUpper;
        uint32_t reserved;

        constexpr GDT64TssEntry(uintptr_t addr)
          : length(sizeof(Tss64)),
            baseLow(addr & 0xFFFF),
            baseMedium((addr >> 16) & 0xFF),
            flags_a(0b10001001),
            flags_b(0),
            baseHigh((addr >> 24) & 0xFF),
            baseUpper((addr >> 32)),
            reserved(0) {}
    } __attribute__((packed)) gdt_tss_entry_t;

    typedef struct GDT64Pack
    {
        GDT64Entry entries[GDT_ENTRY_COUNT] = {};
        GDT64TssEntry tss = {0};
    } __attribute__((packed)) gdt_pack_t;

    extern "C" void asmw_flush_gdt(uint64_t);

    void SetupGDT();
}