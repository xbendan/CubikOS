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

#include <stdint.h>

struct FaultName
{
    char* name;
    uint8_t hasErrorCode;
} FaultNameArrays[] = {
    {"Divide-by-Zero", false},
    {"Debug", false},
    {"Non-maskable Interrupt", false},
    {"Breakpoint", false},
    {"Overflow", false},
    {"Bound Range Exceeded", false},
    {"Invalid Opcode", false},
    {"Device Not Available", false},
    {"Double Fault", true},
    {"Coprocessor Segment Overrun", false},
    {"Invalid TSS", true},
    {"Segment Not Present", true},
    {"Stack-Segment Fault", true},
    {"General Protection Fault", true},
    {"Page Fault", true},
    {NULL, false},
    {"x87 Floating-Point Exception", false},
    {"Alignment Check", false},
    {"Machine Check", false},
    {"SIMD Floating-Point Exception", false},
    {"Virtualization Exception", false},
    {NULL, false},
    {NULL, false},
    {NULL, false},
    {NULL, false},
    {NULL, false},
    {NULL, false},
    {NULL, false},
    {NULL, false},
    {NULL, false},
    {"Security Exception", true},
    {NULL, false}
};

namespace Interrupts {
    typedef struct IDTPointer {
        uint16_t limit;
        uint64_t base;
    } __attribute__((packed)) idt_ptr_t;

    typedef struct IDTEntry {
        uint16_t baseLow;
        uint16_t selector;
        uint8_t ist;
        uint8_t flags;
        uint16_t baseMedium;
        uint32_t baseHigh;
        uint32_t reserved;
    } __attribute__((packed)) idt_entry_t;

    typedef struct RegisterContext {
        uint64_t r15;
        uint64_t r14;
        uint64_t r13;
        uint64_t r12;
        uint64_t r11;
        uint64_t r10;
        uint64_t r9;
        uint64_t r8;
        uint64_t rbp;
        uint64_t rdi;
        uint64_t rsi;
        uint64_t rdx;
        uint64_t rcx;
        uint64_t rbx;
        uint64_t rax;
        uint64_t intno;
        uint64_t err;
        uint64_t rip;
        uint64_t cs;
        uint64_t rflags;
        uint64_t rsp;
        uint64_t ss;
    } __attribute__((packed));

    typedef struct InterruptStackFrame
    {
        uint64_t rip;
        uint64_t cs;
        uint64_t flags;
        uint64_t rsp;
        uint64_t ss;
    }

    void LoadInterruptDescriptorTable();
    void RegisterInterruptHandler(uint8_t intr, isr_t func, void* data);
}