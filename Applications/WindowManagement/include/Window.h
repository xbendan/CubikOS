#include <stdint.h>
#include <stdbool.h>
#include <GraphicsDevice.h>
#include <Graphics/Vector.h>
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
private:
    uint32_t m_Pid;
    WindowHandle *m_WndHandle;
    vector2d_t 
    uint32_t m_Width;
    uint32_t m_Height;
    uint16_t m_CornerRadius;
    /**
     * @brief window attributes
     * 
     */
    uint16_t m_Flags;
    bool m_IsTopmost;

public:
    constexpr Window(uint32_t pid, uint32_t width, uint32_t height) : Window(pid, 0, 0, width, height, CENTER_SCREEN, 4, 0x0, false) {}

    constexpr Window(uint32_t pid, int top, int left, uint32_t width, uint32_t height, WindowStartupLocation showPolicy, uint16_t cornerRadius, uint16_t flags, bool topmost)
      : m_Pid(pid),
        m_Width(width),
        m_Height(height),
        m_CornerRadius(cornerRadius),
        m_Flags(flags),
        m_IsTopmost(topmost)
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

    ~Window();

    void SetTitle(const char* title);
    char *GetTitle();
    void SetTopmost(bool newValue);
    bool IsTopmost();
    void Relocate(vector2d_t vec);
    void Resize(vector2d_t vec);

    void RequestFocus(bool flashTaskbar = true, bool flashSound = false, bool moveToFront = false);
};

namespace WindowManagement
{
    Window* CreateWindow(uint32_t pid, uint32_t width, uint32_t height);
    Window* CreateWindow(uint32_t pid, int top, int left = 0, uint32_t width = 0, uint32_t height, WindowStartupLocation showPolicy = CENTER_SCREEN, )
} // namespace WindowManagement