#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <Console.h>
#include <String.h>

#define MAX_COLUMN 80
#define MAX_ROW 25

namespace Console {
    static inline uint8_t ToColorEntry(enum VGAColor fore, enum VGAColor back) 
    {
        return fore | back << 4;
    }
        
    static inline uint16_t ToBufferEntry(unsigned char code, uint8_t color) 
    {
        return (uint16_t) code | (uint16_t) color << 8;
    }

    size_t c_column;
    size_t c_row;
    uint8_t c_color;
    uint16_t* c_buffer;

    void Initialize(void)
    {
        c_column = 0;
        c_row = 0;
        c_color = ToColorEntry(
            VGA_COLOR_BLACK,
            VGA_COLOR_WHITE
        );
        c_buffer = (uint16_t*) 0x000b8000;

        Clean();
        //PrintLine("Console output initialized.");
    }

    void Clean()
    {
        for(size_t _row = 0; _row < 25; _row++)
        {
            for(size_t _col = 0; _col < 80; _col++)
            {
                c_buffer[_row * MAX_COLUMN + _col] = ToBufferEntry(' ', c_color);
            }
        }
    }

    void NewLine()
    {
        c_row++;
        c_column = 0;

        if(c_row >= MAX_ROW)
        {
            Clean();
            c_row = 0;
        }
    }

    void PrintCode(char code)
    {
        if(c_column == 80) { NewLine(); }

        if(code == '\n') {
            NewLine();
            return;
        }

        c_buffer[c_row * MAX_COLUMN + c_column] = ToBufferEntry(code, c_color);
        c_column++;
    }

    void Print(const char* str)
    {
        size_t len = strlen(str);
        for (size_t index = 0; index < len; index++)
        {
            PrintCode(str[index]);
        }
    }

    void PrintLine(const char* str)
    {
        if(c_column != 0)
            PrintCode('\n');
        Print(str);
        PrintCode('\n');
    }
}

void PrintLine(const char* str)
{
    Console::PrintLine(str);
}

void PrintNum(uint64_t num)
{
    while (num > 0)
    {
        size_t digit = num % 10;
        num /= 10;
        Console::PrintCode((char)(digit + 48));
    }
}