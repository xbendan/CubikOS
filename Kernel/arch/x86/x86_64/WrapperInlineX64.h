static inline void EnableInterrupts() { __asm__("sti"); }
static inline void DisableInterrupts() { __asm__("cli"); }