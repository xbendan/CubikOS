#ifdef ARCH_X86_64
#include <x86_64/ioports.h>
#endif

void CallPanic(const char* text);

void MakeTripleFault();