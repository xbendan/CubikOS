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

namespace Kernel {
namespace Architecture {
    void Halt();
}}
