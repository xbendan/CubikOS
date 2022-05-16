#include <Panic.h>
#include <Graphic/GraphicsDevice.h>

using namespace Graphics;

void Panic(string msg)
{
    DrawRect(
        {0, 0},
        {GetScreen()->width / 2, GetScreen()->height / 2},
        {255, 0, 0},
        0
    );

hang:
    __asm__("hlt");
    goto hang;
}