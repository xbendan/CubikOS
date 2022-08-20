#pragma once

#include <x86_64/cpu.h>

void cli();
void sti();

typedef void (*isr_t)(void*, registers_t*);