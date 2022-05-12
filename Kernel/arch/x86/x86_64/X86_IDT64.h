#define IDT_DIVIDE_BY_ZERO 0x00
#define IDT_SINGLE_STEP 0x01
#define IDT_NMI 0x02
#define IDT_BREAKPOINT 0x03
#define IDT_OVERFLOW 0x04
#define IDT_BOUND_RANGE_EXCEEDED 0x05
#define IDT_INVALID_OPCODE 0x06
#define IDT_COPROCESSOR_NOT_AVAILABLE 0x07
#define IDT_DOUBLE_FAULT 0x08

#define IDT_INVALID_TASK_STATE_SEGMENT 0x0A
#define IDT_SEGMENT_NOT_PRESENT 0x0B
#define IDT_STACK_SEGMENT_FAULT 0x0C
#define IDT_GENERAL_PROTECTION_FAULT 0x0D
#define IDT_PAGE_FAULT 0x0E

#define IDT_X87_FLOATING_POINT_EXCEPTION 0x10
#define IDT_ALIGNMENT_CHECK 0x11
#define IDT_MACHINE_CHECK 0x12
#define IDT_SIMD_FLOATING_POINT_EXCEPTION 0x13
#define IDT_VIRTUALIZATION_EXCEPTION 0x14
#define IDT_CONTROL_PROTECTION_EXCEPTION 0x15

#define IDT_FLAGS_INTGATE 0x8E
#define IDT_FLAGS_TRAPGATE 0xEF
#define IDT_FLAGS_USER 0b01100000

#define IDT_ENTRY_COUNT 256

#include <stdint.h>

namespace Arch::x86_64 {
    typedef struct IDT64Pack {
        uint16_t limit;
        uint64_t base;
    } __attribute__((packed)) idt_ptr_t;

    typedef struct IDT64Entry {
        uint16_t baseLow;
        uint16_t selector;
        uint8_t ist;
        uint8_t flags;
        uint16_t baseMedium;
        uint32_t baseHigh;
        uint32_t reserved;

        constexpr IDT64Entry() : IDT64Entry(0, 0, 0) {}

        constexpr IDT64Entry(uintptr_t base, uint16_t ist, uint8_t flags)
          : baseLow(base & 0xFFFF),
            selector(0x08),
            ist(ist),
            flags(flags),
            baseMedium((base >> 16) & 0xFFFF),
            baseHigh((base >> 32) & 0xFFFFFFFF),
            reserved(0)
        {}

    } __attribute__((packed)) idt_entry_t;

    typedef struct RegisterContext {
        uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
        uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
        uint64_t intno, err;
        uint64_t rip, cs, rflags, rsp, ss;
    } __attribute__((packed)) registers_t;

    typedef void (*isr_t)(void*, registers_t*);

    typedef struct InterruptServiceRoutineHandler {
        isr_t handler;
        void* data;
    } isr_handler_t;

    extern "C" void asmw_flush_idt(uint64_t);

    void SetupIDT();
    void RegisterService(uint8_t intr, isr_t func, void* data = nullptr);
    idt_ptr_t* GetIdtTables();
}