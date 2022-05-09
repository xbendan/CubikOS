#include <Panic.h>
#include <Graphic/GraphicsDevice.h>

using namespace Graphics;

void Panic(string msg)
{
    DrawRect(
        (Point){200, 200},
        (Size){GetScreen()->width / 2, GetScreen()->height / 2},
        (PixelColor){255, 0, 0},
        0
    );

hang:
    __asm__("hlt");
    goto hang;
}