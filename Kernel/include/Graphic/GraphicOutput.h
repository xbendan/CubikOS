typedef struct 
{
    uint32_t Width;
    uint32_t Height;
    
    void* Address;
    uintptr_t AddrPhy;
} DisplayMode;

typedef struct
{
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
    uint8_t Alpha;
} Color;

namespace Kernel {
namespace Graphic {
    void InitGraphicOutput(DisplayMode _dmode);
    DisplayMode GetCurrentDisplayMode();

    class Screen
    {
        uint8_t* gMemory = nullptr;
        uint32_t width;
        uint32_t height;
    }

    void DrawRect(
        unsigned int x, 
        unsigned int y, 
        unsigned int width, 
        unsigned int height,
        Color color
    );
    void DrawChar(
        unsigned int x, 
        unsigned int y, 
        char content,
        Color color
    );
    void DrawText(
        unsigned int x, 
        unsigned int y, 
        const char* content,
        Color color
    );
    void DrawBitmapImage(
        unsigned int x, 
        unsigned int y
    )
}}