#include <GraphicsDevice.h>
#include <GConsole.h>

namespace Graphics
{
    color_t* address;
    screen_t screen;

    void DrawPoint(Point point, PixelColor color)
    {
        
    }

    void DrawLine(Point startPoint, Point endPoint, PixelColor color);
    void DrawRect(Point point, Size size, PixelColor color, uint16_t cornerRadius)
    {
        for (unsigned i = 0; i < size.height; i++) {
            for (unsigned j = 0; j < size.width; j++) {
                screen.buffer[((i + point.y) * screen.width) + j + point.x] = color;
            }
        }
    }

    void DrawCircle(Point centerPoint, int radius, PixelColor color);
    void DrawChar(Point point, PixelColor color, char c)
    {
        uint32_t colour = color.red << 16 | color.green << 8 | color.blue;
        for (unsigned i = 0; i < 8; i++) {
            int row = defaultFont[(int)c][i];
            for (unsigned j = 0; j < 8; j++) {
                if ((row & (1 << j)) >> j) {
                    address[((point.y + i) * screen.width) + point.x + j] = color;
                }
            }
        }
    }

    void DrawText(Point point, PixelColor color, const char* str)
    {
        int xOffset = 0;
        while (*str != 0) {
            DrawChar(Point{point.x + xOffset, point.y}, color, *str);
            xOffset += 8;
            str++;
        }
    }

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
            DrawRect((point_t){10, 10}, (struct Size){200, 100}, color_t(255, 255, 255), 0);
    }
}