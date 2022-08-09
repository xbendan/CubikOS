#ifdef ARCH_X86_64
#include <x86_64/ioports.h>
#endif

void panic(const char* text);

void triple_fault();