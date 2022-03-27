#include <stdint.h>
#define PIT_FREQUENCY 0x1234DE

namespace Arch::x86
{
    void LoadPIT(uint32_t freq);
} // namespace Arch::x86