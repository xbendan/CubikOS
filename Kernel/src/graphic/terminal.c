#include <graphic/terminal.h>
#include <string.h>

const uint16_t color = 15 | 0 << 4;

size_t row;
size_t column;
uint16_t* t_buffer = (uint16_t *) 0xb8000;

void set_t_buffer(uint64_t addr)
{
    t_buffer = (uint16_t *)(addr);
}

void clear_screen()
{
    for (int i = 0; i < 2000; i++)
        t_buffer[i] = ' ' | color << 8;
}

void line_move_up()
{
    for (int i = 0; i < 1920; i++)
        t_buffer[i] = t_buffer[i + 80];
    for (int i = 0; i < 80; i++)
        t_buffer[1920 + i] = ' ' | color << 8;
}

void new_line()
{
    row++;
    column = 0;
    if(row == 25)
    {
        row--;
        line_move_up();
    }
}

void add_char(char c)
{
    t_buffer[row * 80 + column] = ((uint16_t)c | (uint16_t)color << 8);
    column++;
    if(column == 80)
    {
        new_line();
    }
}

void print_string(const char* text)
{
    for(int i = 0; i < strlen(text); i++)
    {
        add_char(text[i]);
    }
    new_line();
}

void print_long(uint64_t val)
{
    for(int i = 0; i < 79; i++)
    {
        add_char((char)(48 + (val % 10)));
        val /= 10;
    }
    new_line();
}