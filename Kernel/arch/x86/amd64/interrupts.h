void disable_interrupts() { __asm__("cli"); }
void enable_interrupts() { __asm__("sti"); }