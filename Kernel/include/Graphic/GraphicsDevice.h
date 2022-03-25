#include <stdint.h>

typedef struct PixelColor
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved;

    PixelColor(uint8_t red, uint8_t green, uint8_t blue)
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

typedef struct Screen
{
    color_t* buffer;
    uint32_t id;
    uint32_t width, height, pitch;
    uint8_t depth;
} screen_t;

namespace Graphics
{
    void DrawPoint(Point point, PixelColor color);
    void DrawLine(Point startPoint, Point endPoint, PixelColor color);
    void DrawRect(Point startPoint, Size size, PixelColor color, uint16_t cornerRadius = 0);
    void DrawCircle(Point centerPoint, int radius, PixelColor color);
    void DrawChar(Point point, PixelColor color, char c);
    void DrawText(Point point, PixelColor color, const char* str);

    screen_t* _screen();

    void Initialize(framebuffer_t fbt);
}

