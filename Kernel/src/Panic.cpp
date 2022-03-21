#include <Panic.h>
#include <GraphicsDevice.h>

using namespace Graphics;

void __attribute__((noreturn)) Panic(string msg)
{
    DrawRect(
        (Point){0, 0},
        (Size){_screen()->width, _screen()->height},
        (ScreenPixelColor){255, 0, 0},
        0
    );
}