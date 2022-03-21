#include <GraphicsDevice.h>

namespace Graphics
{
    framebuffer_t buffer;
    color_t* address;

    void DrawPoint(Point point, ScreenPixelColor color)
    {
        
    }

    void DrawLine(Point startPoint, Point endPoint, ScreenPixelColor color);
    void DrawRect(Point point, Size size, ScreenPixelColor color, uint16_t cornerRadius)
    {
        for (unsigned i = 0; i < size.height; i++) {
            for (unsigned j = 0; j < size.width; j++) {
                address[((i + point.y) * buffer.width) + j + point.x] = color;
            }
        }
    }

    void DrawCircle(Point centerPoint, int radius, ScreenPixelColor color);
    void DrawChar(Point point, ScreenPixelColor color, char c);
    void DrawText(Point point, ScreenPixelColor color, const char* str);

    void Initialize(framebuffer_t fbt)
    {
        buffer = fbt;
        address = (color_t*)fbt.addr;
        if(buffer.depth == 32)
            DrawRect((point_t){10, 10}, (struct Size){100, 100}, color_t(255, 255, 255), 0);
    }
}