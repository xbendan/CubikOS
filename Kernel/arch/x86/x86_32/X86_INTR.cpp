#include <Panic.h>
#include <X86_IDT32.h>
#include <GraphicsDevice.h>

using namespace Arch::x86_32;

int x = 0;

extern "C" uint32_t DispatchInterrupt(uintptr_t esp, RegisterContext regctx)
{
    //Panic("");
    int val = regctx.intno * 8;
    // Ignore graphics related code
    // Its only for debugging LOL
    Graphics::DrawRect(
        (Point){0 + 100 * x, 0},
        (Size){100, 100},
        (PixelColor){val, val, val},
        0
    );
    x++;

    // Double fault if not stop here. :(
    __asm__("hlt");
    return esp;
}