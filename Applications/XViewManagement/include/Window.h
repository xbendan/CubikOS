#include <stdint.h>
#include <stdbool.h>
#include <csystem>
#include <GraphicsDevice.h>
#define WindowHandle uint32_t

enum WindowStartupLocation
{
    CENTER_SCREEN,
    CENTER_OWNER,
    LAST_OPEN,
    TOP_LEFT_CORNER,
    TOP_MEDIUM,
    TOP_RIGHT_CORNER,
    LEFT_MEDIUM,
    RIGHT_MEDIUM,
    BOTTOM_LEFT_CORNER,
    BOTTOM_MEDIUM,
    BOTTOM_RIGHT_CORNER
};

enum WindowAttribute
{

};

class Window
{
    uint32_t process;
    WindowHandle handle;
    int top, left;
    uint32_t width, height;
    uint16_t cornerRadius;
    /**
     * @brief window attributes
     * 
     */
    uint16_t flags;
    bool topmost;

    constexpr Window(uint32_t pid, uint32_t width, uint32_t height) : Window(pid, 0, 0, width, height, CENTER_SCREEN, 4, 0x0, false) {}

    constexpr Window(uint32_t pid, int top, int left, uint32_t width, uint32_t height, WindowStartupLocation showPolicy, uint16_t cornerRadius, uint16_t flags, bool topmost)
      : process(pid),
        width(width),
        height(height),
        cornerRadius(cornerRadius),
        flags(flags),
        topmost(topmost)
    {
        if(showPolicy == CENTER_OWNER)
        {

        }
        else
        {
            
            switch (showPolicy)
            {
            case /* constant-expression */:
                /* code */
                break;
            
            default:
                break;
            }
        }
            
    }

    public:
        void SetTopmost(bool newValue) { topmost = newValue; }
        bool IsTopmost() { return topmost; }
        void SetTopPosition(int newValue) { top = newValue; }
        int GetTopPosition() { return top; }
        void SetLeftPosition(int newValue) { left = newValue; }
        int GetLeftPosition() { return left; }

        void RequestFocus(bool flashTaskbar = true, bool flashSound = false, bool moveToFront = false);
};

namespace WindowManagement
{
    Window* CreateWindow(uint32_t pid, uint32_t width, uint32_t height);
    Window* CreateWindow(uint32_t pid, int top, int left = 0, uint32_t width = 0, uint32_t height, WindowStartupLocation showPolicy = CENTER_SCREEN, )
} // namespace WindowManagement