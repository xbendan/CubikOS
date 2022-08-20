#pragma once

#include <x86_64/cpu.h>

void DisableInterrupts();
void EnableInterrupts();

typedef void (*isr_t)(void*, registers_t*);