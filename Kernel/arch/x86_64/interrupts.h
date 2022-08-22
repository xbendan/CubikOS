#pragma once

#include <x86_64/cpu.h>

void DisableInterrupts();
void EnableInterrupts();

typedef void (*isr_t)(void*, registers_t*);

void RegisterInterruptHandler(int num, isr_t func);