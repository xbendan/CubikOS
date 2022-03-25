#include <Panic.h>
#include <X86_IDT32.h>
#include <GraphicsDevice.h>

using namespace Arch::x86_32;

extern "C" uint32_t DispatchInterrupt(uintptr_t esp, RegisterContext regctx)
{
    //Panic("");
    int val = regctx.intno * 8;
    Graphics::DrawRect(
        (Point){0, 0},
        (Size){100, 100},
        (PixelColor){val, val, val},
        0
    );
    __asm__("hlt");
    return esp;
}