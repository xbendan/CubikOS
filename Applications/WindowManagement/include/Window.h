#include <stdint.h>
#include <stdbool.h>
#include <GraphicsDevice.h>
#include <Graphics/Vector.h>
#define WindowHandle uint32_t

enum WindowStartupLocation
{
    WindowStartupCenterScreen,
    WindowStartupCenterOwner,
    WindowStartupLastOpen,
    WindowStartupTopLeft,
    WindowStartupTopMedium,
    WindowStartupTopRight,
    WindowStartupLeft,
    WindowStartupRight,
    WindowStartupBottomLeft,
    WindowStartupBottomMedium,
    WindowStartupBottomRight
};

enum WindowAttribute
{

};

struct WindowBitmapBuffer
{
    uint32_t buffer;
    uint64_t
};

class Window
{
private:
    uint32_t m_Pid;
    WindowHandle *m_WndHandle;
    vector2d_t m_Position;
    size2d_t m_Size;
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
        m_CornerRadius(cornerRadius),
        m_Flags(flags),
        m_IsTopmost(topmost)
    {
        m_Size = (size2d_t) { .width = width, .height = height };

        switch (showPolicy)
        {
        case WindowStartupCenterScreen:
            /* code */
            break;
        case WindowStartupCenterOwner:
            break;
        default:
            break;
        }
    }

    ~Window();

    void SetTitle(const char* title);
    char *GetTitle();
    void SetTopmost(bool newValue);
    bool IsTopmost();
    vector2d_t GetPosition();
    void Relocate(vector2d_t vec);
    void Resize(vector2d_t vec);

    void RequestFocus(bool flashTaskbar = true, bool flashSound = false, bool moveToFront = false);
};

namespace WindowManagement
{
    Window* CreateWindow(uint32_t pid, uint32_t width, uint32_t height);
    Window* CreateWindow(uint32_t pid, int top, int left = 0, uint32_t width = 0, uint32_t height, WindowStartupLocation showPolicy = CENTER_SCREEN, )
} // namespace WindowManagement