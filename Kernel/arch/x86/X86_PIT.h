#include <stdint.h>
#define PIT_FREQUENCY 0x1234DE

namespace Arch::x86
{
    void LoadPit(uint32_t freq);
} // namespace Arch::x86