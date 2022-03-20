#include <stdint.h>

class GraphicsDevice
{
    /* data */
};


class Screen : public GraphicsDevice
{
    uint64_t id;
    uint32_t width, height;
};

namespace Graphic
{
    void DrawPoint(int x, int y);
}

