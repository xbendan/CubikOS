enum VGAColor {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

namespace Console {
    enum LogType
    {
        INFO = 0,
        WARN = 1,
        SUCCESS = 2,
        ERROR = 3
    };

    void Initialize(void);
    void Clean(void);
    void PrintCode(char code);
    void Print(const char* str);
    void PrintLine(const char* str);
}
void PrintLine(const char* str);
void PrintNum(uint64_t num);
