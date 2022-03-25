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

namespace Arch::x86_32 {
    typedef struct IDT32Pack {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) idt_ptr_t;

    typedef struct IDT32Entry {
        uint16_t baseLow;
        uint16_t selector;
        uint8_t ign;
        uint8_t flags;
        uint16_t baseMedium;

        constexpr IDT32Entry() : IDT32Entry(0, 0, 0) {}

        constexpr IDT32Entry(uintptr_t base, uint16_t selector, uint8_t flags)
          : baseLow(base & 0xFFFF),
            selector(selector),
            ign(0),
            flags(flags),
            baseMedium((base >> 16) & 0xFFFF) {}

    } __attribute__((packed)) idt_entry_t;

    typedef struct RegisterContext {
        uint32_t r15;
        uint32_t r14;
        uint32_t r13;
        uint32_t r12;
        uint32_t r11;
        uint32_t r10;
        uint32_t r9;
        uint32_t r8;
        uint32_t rbp;
        uint32_t rdi;
        uint32_t rsi;
        uint32_t rdx;
        uint32_t rcx;
        uint32_t rbx;
        uint32_t rax;
        uint32_t intno;
        uint32_t err;
        uint32_t rip;
        uint32_t cs;
        uint32_t rflags;
        uint32_t rsp;
        uint32_t ss;
    } __attribute__((packed)) reg_context_t;

    typedef void (*isr_t)(void*, RegisterContext*);

    typedef struct InterruptServiceRoutineHandler {
        isr_t handler;
        void* data;
    } isr_handler_t;

    extern "C" void __flushIDT(uint32_t);

    void SetupIDT();
    void RegisterService(uint8_t intr, isr_t func, void* data = nullptr);
    void SetIDTEntry(uint8_t vec, uint32_t base, uint16_t selector, uint8_t flags);
}