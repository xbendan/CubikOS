#include <stdint.h>

typedef struct ScreenPixelColor
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved;

    ScreenPixelColor(uint8_t red, uint8_t green, uint8_t blue)
      : blue(blue),
        green(green),
        red(red) {}
} __attribute__((packed)) color_t;

typedef struct Point
{
    int x, y;
} point_t;

struct Size
{
    uint32_t width, height;
};

typedef struct FramebufferDetail
{
    uint64_t addr;
    uint32_t pitch, height, width;
    uint8_t depth; // 32 in QEMU
} framebuffer_t;

class GraphicsDevice
{
    /* data */
};

class Screen : public GraphicsDevice
{
    framebuffer_t buffer;
    uint64_t id;
    uint32_t width, height;
};

namespace Graphics
{
    void DrawPoint(Point point, ScreenPixelColor color);
    void DrawLine(Point startPoint, Point endPoint, ScreenPixelColor color);
    void DrawRect(Point startPoint, Point endPoint, ScreenPixelColor color, uint16_t cornerRadius = 0);
    void DrawCircle(Point centerPoint, int radius, ScreenPixelColor color);
    void DrawChar(Point point, ScreenPixelColor color, char c);
    void DrawText(Point point, ScreenPixelColor color, const char* str);

    void EnableGraphicOutput(framebuffer_t fbt);
}

