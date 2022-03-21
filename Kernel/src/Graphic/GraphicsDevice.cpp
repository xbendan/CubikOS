#include <GraphicsDevice.h>

namespace Graphics
{
    color_t* address;
    screen_t screen;

    void DrawPoint(Point point, ScreenPixelColor color)
    {
        
    }

    void DrawLine(Point startPoint, Point endPoint, ScreenPixelColor color);
    void DrawRect(Point point, Size size, ScreenPixelColor color, uint16_t cornerRadius)
    {
        for (unsigned i = 0; i < size.height; i++) {
            for (unsigned j = 0; j < size.width; j++) {
                screen.buffer[((i + point.y) * screen.width) + j + point.x] = color;
            }
        }
    }

    void DrawCircle(Point centerPoint, int radius, ScreenPixelColor color);
    void DrawChar(Point point, ScreenPixelColor color, char c);
    void DrawText(Point point, ScreenPixelColor color, const char* str);

    screen_t* _screen()
    {
        return &screen;
    }

    void Initialize(framebuffer_t fbt)
    {
        screen = {
            .buffer = (color_t*)fbt.addr,
            .id = 0,
            .width = fbt.width,
            .height = fbt.height,
            .pitch = fbt.pitch,
            .depth = fbt.depth
        };
        if(fbt.depth == 32)
            DrawRect((point_t){10, 10}, (struct Size){100, 100}, color_t(255, 255, 255), 0);
    }
}