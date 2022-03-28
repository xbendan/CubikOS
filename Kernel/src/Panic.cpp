#include <Panic.h>
#include <GraphicsDevice.h>

using namespace Graphics;

void Panic(string msg)
{
    DrawRect(
        (Point){200, 200},
        (Size){_screen()->width / 2, _screen()->height / 2},
        (PixelColor){255, 0, 0},
        0
    );

hang:
    __asm__("hlt");
    goto hang;
}