#include <IntrTable.h>

namespace Kernel {
namespace Architecture {
    void Halt()
    {
        asm("cli");
        asm("hlt");
    }
}}