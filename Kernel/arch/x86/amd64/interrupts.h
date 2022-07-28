#pragma once

void cli();
void sti();

typedef struct registers
{
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
    uint64_t intno, err;
    uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed)) registers_t;

typedef void (*isr_t)(void*, registers_t*);